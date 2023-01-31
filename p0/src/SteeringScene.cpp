#include "pch.h"
#include "SteeringScene.h"
#include "SteeringFactory.h"
#include "BuildingFactory.h"
#include "PlayerFactory.h"

#include "PlayerSystem.h"
#include "SteeringSystem.h"
#include "DynamicsSystem.h"
#include "CollisionSystem.h"

#include "Steering.h"
#include "SteeringEntity.h"
#include "DebugRenderer.h"

#include "Utility.h"

#define MAZE false
#define CHICKEN false
#define BEHAVIOURS true
#define AVOID_AHEAD true
#define SPEED_MAX 1000.0f

namespace
{
	// Player capsule collider dimensions
	constexpr float r = 33.0f;
	constexpr float hh = 43.0f;
	constexpr float sphereRadius = 20.0f;
	constexpr float sensorRadius = sphereRadius * 2.0f;
}

using namespace DirectX;

void SteeringScene::Reset()
{
	EntityTransform& t1 = *mComponents.transforms.GetComponent(mAvoider1);
	EntityTransform& t2 = *mComponents.transforms.GetComponent(mAvoider2);

	t1.Translate({ Random(0.0f, mWorldWidth), Random(0.0f, mWorldHeight), 0.0f });
	t2.Translate({ Random(0.0f, mWorldWidth), Random(0.0f, mWorldHeight), 0.0f });
	while (((t2.WorldPosition() - t1.WorldPosition()).Length() < 1000.0f))
	{
		t1.Translate({ Random(0.0f, mWorldWidth), Random(0.0f, mWorldHeight), 0.0f });
		t2.Translate({ Random(0.0f, mWorldWidth), Random(0.0f, mWorldHeight), 0.0f });
	}

	Vector3 v12 = t2.WorldPosition() - t1.WorldPosition();
	Vector3 v21 = t1.WorldPosition() - t2.WorldPosition();
	t1.Orientate(v12);
	t2.Orientate(v21);
}

SteeringScene::SteeringScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components)
	: Scene(graphics, audio, components)
{
	mMap = CreateMap(Map::MINTY_AFTERSHAVE, components, sBuildingRenderer, mWorldWidth, mWorldHeight);

#if BEHAVIOURS
	mSeeker = CreateSteering(mComponents, SteeringBehaviour::SEEK, SPEED_MAX, sPlayer);
	mArriver = CreateSteering(mComponents, SteeringBehaviour::ARRIVE, SPEED_MAX, sPlayer);
	mWanderer = CreateEntity(mComponents, mWorldWidth * 0.5f, mWorldHeight * 0.5f);
	mComponents.rigidbodies.Add(mWanderer);
#endif
	
#if MAZE
	mRandomTarget = CreateEntity(mComponents, Random(0.0f, mWorldWidth), Random(0.0f, mWorldHeight));
	mAvoidingSeeker = CreateEntity(mComponents, Random(0.0f, mWorldWidth), Random(0.0f, mWorldHeight));
	mSensor = CreateEntity(mComponents, 0.0f, sphereRadius + sensorRadius);
	mComponents.rigidbodies.Add(mRandomTarget);
	mComponents.rigidbodies.Add(mAvoidingSeeker);
	mComponents.rigidbodies.Add(mSensor);
#if AVOID_AHEAD
	AddSphere(mSensor, sensorRadius, mComponents);
	AddChild(mAvoidingSeeker, mSensor, mComponents);
#else
	AddSphere(mAvoidingSeeker, 50.0f, mComponents);
#endif

	AddTimer("RandomTarget", 5.0f, [&] {
		mComponents.transforms.GetComponent(mRandomTarget)->Translate
		(
			Random(0.0f, mWorldWidth),
			Random(0.0f, mWorldHeight),
			0.0f
		);
	}, true);
#endif

#if CHICKEN
	mAvoider1 = CreateSteering(mComponents, SteeringBehaviour::AVOID, SPEED_MAX);
	mAvoider2 = CreateSteering(mComponents, SteeringBehaviour::AVOID, SPEED_MAX);
	mComponents.steering.GetComponent(mAvoider1)->target = mAvoider2;
	mComponents.steering.GetComponent(mAvoider2)->target = mAvoider1;
	AddCapsule(mAvoider1, r, hh, mComponents);
	AddCapsule(mAvoider2, r, hh, mComponents);
	Reset();

	Entity collider1 = CreateEntity(mComponents, 0.0f, r * 3.0f + hh);
	Entity collider2 = CreateEntity(mComponents, 0.0f, r * 3.0f + hh);
	AddCapsule(collider1, r, hh * 2.0f, mComponents);
	AddCapsule(collider2, r, hh * 2.0f, mComponents);
	AddChild(mAvoider1, collider1, mComponents);
	AddChild(mAvoider2, collider2, mComponents);

	AddTimer("Reset", 2.0f, [&] {
		Reset();
	}, true);
#endif
}

SteeringScene::~SteeringScene()
{
}

void SteeringScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 60.0f * XM_RADIANS;
	fovAngleY = aspectRatio < 1.0f ? fovAngleY * 2.0f : fovAngleY;
	mView = Matrix::CreateLookAt(
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 1000.0f },
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f },
		Vector3::Up);
	mProj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.1f, 10000.0f);
}

void SteeringScene::OnBegin()
{
}

void SteeringScene::OnEnd()
{
}

void SteeringScene::OnPause()
{
}

void SteeringScene::OnResume()
{
}

void SteeringScene::OnUpdate(float dt, float tt, const DX::Input& input)
{
#if BEHAVIOURS
	{
		Rigidbody& rb = *mComponents.rigidbodies.GetComponent(mWanderer);
		EntityTransform& transform = *mComponents.transforms.GetComponent(mWanderer);
		if (transform.Translation().x > mWorldWidth || transform.Translation().x < 0.0f ||
			transform.Translation().y > mWorldHeight || transform.Translation().y < 0.0f)
		{
			rb.acceleration = Vector3::Zero;
			rb.velocity = Vector3::Zero;
			transform.Translate(mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f);
			transform.Orientate(RandomSpherePoint(1.0f));
		}
		Steering::Wander(mWanderer, 1000.0f, 500.0f, mComponents);
		rb.acceleration.z = 0.0f;
	}
#endif

#if MAZE
	struct Hit
	{
		Entity entity;
		Vector3 mtv;
	};
	std::vector<Hit> collisions;
	for (Entity building : mMap)
	{
		Entity buildingCollider = *mComponents.hierarchies.GetComponent(building)->children.begin();
		Hit hit;
#if AVOID_AHEAD
		if (Collision::IsColliding(buildingCollider, mSensor, hit.mtv, mComponents))
#else
		if (Collision::IsColliding(buildingCollider, mAvoidingSeeker, hit.mtv, mComponents))
#endif
		{
			hit.entity = buildingCollider;
			collisions.push_back(hit);
		}
	}

	if (collisions.empty())
	{
		Steering::Arrive(mRandomTarget, mAvoidingSeeker, SPEED_MAX, dt, mComponents);
	}
	else
	{
		EntityTransform& transform = *mComponents.transforms.GetComponent(mAvoidingSeeker);
		Vector3 p = transform.WorldPosition();
		std::sort(collisions.begin(), collisions.end(),
			[&](const Hit& a, const Hit& b)
			{
				Vector3 vA = mComponents.transforms.GetComponent(a.entity)->WorldPosition();
				Vector3 vB = mComponents.transforms.GetComponent(a.entity)->WorldPosition();
				return (vA - p).LengthSquared() < (vB - p).LengthSquared();
			}
		);

		Rigidbody& rb = *mComponents.rigidbodies.GetComponent(mAvoidingSeeker);
		rb.acceleration = Steering::Seek(p + collisions[0].mtv, p, rb.velocity, SPEED_MAX);
	}
#endif

	Players::Update(mComponents, input, dt);
	Steering::Update(mComponents, dt);
	Dynamics::Update(mComponents, dt);
}

void SteeringScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto drawSphere = [&](Entity entity, float radius,
		XMVECTOR color = Colors::White, bool wireframe = false)
	{
		Debug::Sphere(mComponents.transforms.GetComponent(entity)->WorldPosition(),
			radius, mView, mProj, graphics, color, wireframe);
	};

	auto drawCapsule = [&](Entity entity, float radius, float halfHeight,
		XMVECTOR color = Colors::White, bool wireframe = false)
	{
		EntityTransform& transform = *mComponents.transforms.GetComponent(entity);
		Debug::Capsule(transform.WorldPosition(), transform.WorldForward(),
			radius, halfHeight, mView, mProj, graphics, color, wireframe);
	};

#if BEHAVIOURS
	sPlayerRenderer.Render(mComponents.transforms.GetComponent(sPlayer)->World(), mView, mProj, graphics);
	drawSphere(mSeeker, r);
	drawSphere(mArriver, r, Colors::PowderBlue);
	drawSphere(mWanderer, r, Colors::MediumPurple);
#endif

#if MAZE
	for (Entity building : mMap)
	{
		Debug::Sphere(
			mComponents.transforms.GetComponent(building)->WorldPosition(),
			45.0f, mView, mProj, graphics, Colors::Black);
	}

#if AVOID_AHEAD
	drawSphere(mAvoidingSeeker, sphereRadius, Colors::MediumAquamarine);
	drawSphere(mSensor, sensorRadius, Colors::Lime, true);
#else
	drawSphere(mAvoidingSeeker, mComponents.colliders.GetComponent(mAvoidingSeeker)->r, Colors::MediumAquamarine);
#endif
	drawSphere(mRandomTarget, r, Colors::MediumOrchid);
#endif

#if CHICKEN
	sPlayerRenderer.Render(mComponents.transforms.GetComponent(mAvoider1)->World(), mView, mProj, graphics);
	sPlayerRenderer.Render(mComponents.transforms.GetComponent(mAvoider2)->World(), mView, mProj, graphics);

	// Uncomment to render sensor colliders
	Entity child1 = *mComponents.hierarchies.GetComponent(mAvoider1)->children.begin();
	Entity child2 = *mComponents.hierarchies.GetComponent(mAvoider2)->children.begin();
	Collider& avoidCollider1 = *mComponents.colliders.GetComponent(child1);
	Collider& avoidCollider2 = *mComponents.colliders.GetComponent(child2);
	drawCapsule(child1, avoidCollider1.r, avoidCollider1.hh, Colors::Gray, true);
	drawCapsule(child2, avoidCollider2.r, avoidCollider2.hh, Colors::Gray, true);
#endif
}