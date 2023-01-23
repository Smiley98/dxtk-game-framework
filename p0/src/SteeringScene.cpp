#include "pch.h"
#include "SteeringScene.h"
#include "SteeringFactory.h"
#include "BuildingFactory.h"
#include "PlayerFactory.h"

#include "PlayerSystem.h"
#include "SteeringSystem.h"
#include "DynamicsSystem.h"

#include "Utility.h"

#define SEEK_PLAYER false

namespace
{
	// Player capsule collider dimensions
	constexpr float r = 33.0f;
	constexpr float hh = 43.0f;
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

#if SEEK_PLAYER
	mRandomTarget = CreateEntity(mComponents);
	mComponents.rigidbodies.Add(mRandomTarget);

	mSeeker = CreateSteering(mComponents, SteeringBehaviour::SEEK, 1000.0f, sPlayer);
	mArriver = CreateSteering(mComponents, SteeringBehaviour::ARRIVE, 1000.0f, sPlayer);
	mRandomSeeker = CreateSteering(mComponents, SteeringBehaviour::SEEK, 1000.0f, mRandomTarget);

	AddTimer("RandomTarget", 1.0f, [&] {
		mComponents.transforms.GetComponent(mRandomTarget)->Translate
		(
			Random(0.0f, mWorldWidth),
			Random(0.0f, mWorldHeight),
			0.0f
		);
	}, true);
#else
	mAvoider1 = CreateSteering(mComponents, SteeringBehaviour::AVOID, 1000.0f);
	mAvoider2 = CreateSteering(mComponents, SteeringBehaviour::AVOID, 1000.0f);
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

	sBuildingRenderer.DebugMap(mMap, mComponents, mView, mProj, graphics, true);

#if SEEK_PLAYER
	drawSphere(mSeeker, r);
	drawSphere(mArriver, r, Colors::PowderBlue);
	drawSphere(mRandomSeeker, r, Colors::MediumAquamarine);
	drawSphere(mRandomTarget, r, Colors::MediumOrchid);
	drawSphere(mWanderer, r, Colors::MediumPurple);

	Vector3 wandererPosition = mComponents.transforms.GetComponent(mWanderer)->Translation();
	if (wandererPosition.x > mWorldWidth  || wandererPosition.x < 0.0f ||
		wandererPosition.y > mWorldHeight || wandererPosition.y < 0.0f)
	{
		mComponents.transforms.GetComponent(mWanderer)->Translate(mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f);
	}
#else
	sPlayerRenderer.Render(mComponents.transforms.GetComponent(mAvoider1)->World(), mView, mProj, graphics);
	sPlayerRenderer.Render(mComponents.transforms.GetComponent(mAvoider2)->World(), mView, mProj, graphics);

	// Uncomment to render sensor colliders
	//Entity child1 = *mComponents.hierarchies.GetComponent(mAvoider1)->children.begin();
	//Entity child2 = *mComponents.hierarchies.GetComponent(mAvoider2)->children.begin();
	//Collider& avoidCollider1 = *mComponents.colliders.GetComponent(child1);
	//Collider& avoidCollider2 = *mComponents.colliders.GetComponent(child2);
	//drawCapsule(child1, avoidCollider1.r, avoidCollider1.hh, Colors::Gray, true);
	//drawCapsule(child2, avoidCollider2.r, avoidCollider2.hh, Colors::Gray, true);
#endif
}
