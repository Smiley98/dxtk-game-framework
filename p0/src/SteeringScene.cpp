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
#include "EntityFunctions.h"

#include "DebugRenderer.h"
#include "Utility.h"

#define MAZE true
#define CHICKEN false
#define BEHAVIOURS false
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
	EntityTransform& t1 = sComponents.GetTransform(mAvoider1);
	EntityTransform& t2 = sComponents.GetTransform(mAvoider2);

	t1.Translate({ Random(0.0f, mSpace.worldWidth), Random(0.0f, mSpace.worldHeight), 0.0f });
	t2.Translate({ Random(0.0f, mSpace.worldWidth), Random(0.0f, mSpace.worldHeight), 0.0f });
	while (((t2.WorldPosition() - t1.WorldPosition()).Length() < 1000.0f))
	{
		t1.Translate({ Random(0.0f, mSpace.worldWidth), Random(0.0f, mSpace.worldHeight), 0.0f });
		t2.Translate({ Random(0.0f, mSpace.worldWidth), Random(0.0f, mSpace.worldHeight), 0.0f });
	}

	Vector3 v12 = t2.WorldPosition() - t1.WorldPosition();
	Vector3 v21 = t1.WorldPosition() - t2.WorldPosition();
	t1.Orientate(v12);
	t2.Orientate(v21);
}

SteeringScene::SteeringScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	: Scene(graphics, audio)
{
	mMap = CreateMap(Map::MINTY_AFTERSHAVE, sComponents, sBuildingRenderer, mSpace.worldWidth, mSpace.worldHeight);

#if BEHAVIOURS
	mSeeker = CreateSteering(sComponents, SteeringBehaviour::SEEK, SPEED_MAX, sPlayer);
	mArriver = CreateSteering(sComponents, SteeringBehaviour::ARRIVE, SPEED_MAX, sPlayer);
	mWanderer = CreateEntity(sComponents, mSpace.worldWidth * 0.5f, mSpace.worldHeight * 0.5f);
	sComponents.rigidbodies.Add(mWanderer);
#endif
	
#if MAZE
	mRandomTarget = CreateEntity(sComponents, Random(0.0f, mSpace.worldWidth), Random(0.0f, mSpace.worldHeight));
	mAvoidingSeeker = CreateEntity(sComponents, Random(0.0f, mSpace.worldWidth), Random(0.0f, mSpace.worldHeight));
	mSensor = CreateEntity(sComponents, 0.0f, sphereRadius + sensorRadius);
	sComponents.rigidbodies.Add(mRandomTarget);
	sComponents.rigidbodies.Add(mAvoidingSeeker);
	sComponents.rigidbodies.Add(mSensor);
#if AVOID_AHEAD
	AddSphere(mSensor, sensorRadius, sComponents);
	AddChild(mAvoidingSeeker, mSensor, sComponents);
#else
	AddSphere(mAvoidingSeeker, 50.0f, sComponents);
#endif

	AddTimer("RandomTarget", 5.0f, [&] {
		sComponents.GetTransform(mRandomTarget).Translate
		(
			Random(0.0f, mSpace.worldWidth),
			Random(0.0f, mSpace.worldHeight),
			0.0f
		);
	}, true);
#endif

#if CHICKEN
	mAvoider1 = CreateSteering(sComponents, SteeringBehaviour::AVOID, SPEED_MAX);
	mAvoider2 = CreateSteering(sComponents, SteeringBehaviour::AVOID, SPEED_MAX);
	sComponents.GetSteering(mAvoider1).target = mAvoider2;
	sComponents.GetSteering(mAvoider2).target = mAvoider1;
	AddCapsule(mAvoider1, r, hh, sComponents);
	AddCapsule(mAvoider2, r, hh, sComponents);
	Reset();

	Entity collider1 = CreateEntity(sComponents, 0.0f, r * 3.0f + hh);
	Entity collider2 = CreateEntity(sComponents, 0.0f, r * 3.0f + hh);
	AddCapsule(collider1, r, hh * 2.0f, sComponents);
	AddCapsule(collider2, r, hh * 2.0f, sComponents);
	AddChild(mAvoider1, collider1, sComponents);
	AddChild(mAvoider2, collider2, sComponents);

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
	mSpace.view = Matrix::CreateLookAt(
		{ mSpace.worldWidth * 0.5f, mSpace.worldHeight * 0.5f, 1000.0f },
		{ mSpace.worldWidth * 0.5f, mSpace.worldHeight * 0.5f, 0.0f },
		Vector3::Up);
	mSpace.proj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.1f, 10000.0f);
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

void SteeringScene::OnUpdate(float dt, float tt)
{
#if BEHAVIOURS
	{
		Rigidbody& rb = sComponents.GetRigidbody(mWanderer);
		EntityTransform& transform = sComponents.GetTransform(mWanderer);
		if (transform.Translation().x > mSpace.worldWidth || transform.Translation().x < 0.0f ||
			transform.Translation().y > mSpace.worldHeight || transform.Translation().y < 0.0f)
		{
			rb.acceleration = Vector3::Zero;
			rb.velocity = Vector3::Zero;
			transform.Translate(mSpace.worldWidth * 0.5f, mSpace.worldHeight * 0.5f, 0.0f);
			transform.Orientate(RandomSpherePoint(1.0f));
		}
		Wander(mWanderer, 1000.0f, 500.0f, sComponents);
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
		Entity buildingCollider = *sComponents.GetHierarchy(building).children.begin();
		Hit hit;
#if AVOID_AHEAD
		if (Collision::IsColliding(buildingCollider, mSensor, hit.mtv, sComponents))
#else
		if (Collision::IsColliding(buildingCollider, mAvoidingSeeker, hit.mtv, sComponents))
#endif
		{
			hit.entity = buildingCollider;
			collisions.push_back(hit);
		}
	}

	if (collisions.empty())
	{
		Arrive(mRandomTarget, mAvoidingSeeker, SPEED_MAX, dt, sComponents);
	}
	else
	{
		EntityTransform& transform = sComponents.GetTransform(mAvoidingSeeker);
		Vector3 p = transform.WorldPosition();
		std::sort(collisions.begin(), collisions.end(),
			[&](const Hit& a, const Hit& b)
			{
				Vector3 vA = sComponents.GetTransform(a.entity).WorldPosition();
				Vector3 vB = sComponents.GetTransform(a.entity).WorldPosition();
				return (vA - p).LengthSquared() < (vB - p).LengthSquared();
			}
		);

		Rigidbody& rb = sComponents.GetRigidbody(mAvoidingSeeker);
		rb.acceleration = Steering::Seek(p + collisions[0].mtv, p, rb.velocity, SPEED_MAX);
	}
#endif

	Players::Update(sComponents, dt);
	Steering::Update(sComponents, dt);
	Dynamics::Update(sComponents, dt);
}

void SteeringScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto drawSphere = [&](Entity entity, float radius,
		XMVECTOR color = Colors::White, bool wireframe = false)
	{
		Debug::DrawSphere(sComponents.GetTransform(entity).WorldPosition(), radius, color, wireframe);
	};

	auto drawCapsule = [&](Entity entity, float radius, float halfHeight,
		XMVECTOR color = Colors::White, bool wireframe = false)
	{
		EntityTransform& transform = sComponents.GetTransform(entity);
		Debug::DrawCapsule(transform.WorldPosition(), transform.WorldForward(), radius, halfHeight, color, wireframe);
	};

#if BEHAVIOURS
	sPlayerRenderer.Render(sComponents.GetTransform(sPlayer).World(), mSpace.view, mSpace.proj, graphics);
	drawSphere(mSeeker, r);
	drawSphere(mArriver, r, Colors::PowderBlue);
	drawSphere(mWanderer, r, Colors::MediumPurple);
#endif

#if MAZE
	for (Entity building : mMap)
	{
		//sBuildingRenderer.DebugBuilding(building, sComponents, mSpace.view, mSpace.proj, graphics);
		Debug::DrawSphere(sComponents.GetTransform(building).WorldPosition(), 45.0f, Colors::Black);
	}

#if AVOID_AHEAD
	drawSphere(mAvoidingSeeker, sphereRadius, Colors::MediumAquamarine);
	drawSphere(mSensor, sensorRadius, Colors::Lime, true);
#else
	drawSphere(mAvoidingSeeker, sComponents.GetCollider(mAvoidingSeeker).r, Colors::MediumAquamarine);
#endif
	drawSphere(mRandomTarget, r, Colors::MediumOrchid);
#endif

#if CHICKEN
	sPlayerRenderer.Render(sComponents.GetTransform(mAvoider1).World(), mSpace.view, mSpace.proj, graphics);
	sPlayerRenderer.Render(sComponents.GetTransform(mAvoider2).World(), mSpace.view, mSpace.proj, graphics);

	// Uncomment to render sensor colliders
	Entity child1 = *sComponents.GetHierarchy(mAvoider1).children.begin();
	Entity child2 = *sComponents.GetHierarchy(mAvoider2).children.begin();
	Collider& avoidCollider1 = sComponents.GetCollider(child1);
	Collider& avoidCollider2 = sComponents.GetCollider(child2);
	drawCapsule(child1, avoidCollider1.r, avoidCollider1.hh, Colors::Gray, true);
	drawCapsule(child2, avoidCollider2.r, avoidCollider2.hh, Colors::Gray, true);
#endif
}
