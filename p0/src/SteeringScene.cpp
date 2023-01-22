#include "pch.h"
#include "SteeringScene.h"
#include "SteeringFactory.h"
#include "PlayerFactory.h"

#include "PlayerSystem.h"
#include "SteeringSystem.h"
#include "DynamicsSystem.h"

#include "Utility.h"

namespace
{
	constexpr float r = 50.0f;
	constexpr float hh = 150.0f;
}

using namespace DirectX;

SteeringScene::SteeringScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components)
	: Scene(graphics, audio, components)
{
	mRandomTarget = CreateEntity(mComponents);
	mComponents.rigidbodies.Add(mRandomTarget);

	mSeeker = CreateSteering(mComponents, SteeringBehaviour::SEEK, 1000.0f, sPlayer);
	mArriver = CreateSteering(mComponents, SteeringBehaviour::ARRIVE, 1000.0f, sPlayer);
	mRandomSeeker = CreateSteering(mComponents, SteeringBehaviour::SEEK, 1000.0f, mRandomTarget);

	mTarget1 = CreateEntity(mComponents, mWorldWidth * 0.01f, mWorldHeight * 0.5f);
	mTarget2 = CreateEntity(mComponents, mWorldWidth * 0.99f, mWorldHeight * 0.5f);
	AddSphere(mTarget1, r, mComponents);
	AddSphere(mTarget2, r, mComponents);

	mAvoider1 = CreateSteering(mComponents, SteeringBehaviour::AVOID, 1000.0f, mTarget1);
	mAvoider2 = CreateSteering(mComponents, SteeringBehaviour::AVOID, 1000.0f, mTarget2);
	AddSphere(mAvoider1, r, mComponents);
	AddSphere(mAvoider2, r, mComponents);
	mComponents.transforms.GetComponent(mAvoider1)->RotateZ(-90.0f);
	mComponents.transforms.GetComponent(mAvoider2)->RotateZ(90.0f);
	mComponents.transforms.GetComponent(mAvoider1)->Translate(mWorldWidth * 0.45f, mWorldHeight * 0.5f, 0.0f);
	mComponents.transforms.GetComponent(mAvoider2)->Translate(mWorldWidth * 0.55f, mWorldHeight * 0.5f, 0.0f);

	Entity collider1 = CreateEntity(mComponents, 0.0f, r + hh);
	Entity collider2 = CreateEntity(mComponents, 0.0f, r + hh);
	AddCapsule(collider1, r, hh, mComponents);
	AddCapsule(collider2, r, hh, mComponents);
	AddChild(mAvoider1, collider1, mComponents);
	AddChild(mAvoider2, collider2, mComponents);

	AddTimer("RandomTarget", 1.0f, [&] {
		mComponents.transforms.GetComponent(mRandomTarget)->Translate
		(
			Random(0.0f, mWorldWidth),
			Random(0.0f, mWorldHeight),
			0.0f
		);
		}, true);

	AddTimer("targets", 2.0f, [&] {
		static bool flip;
	if (flip)
	{
		mComponents.steering.GetComponent(mAvoider1)->target = mAvoider2;
		mComponents.steering.GetComponent(mAvoider2)->target = mAvoider1;
	}
	else
	{
		mComponents.transforms.GetComponent(mTarget1)->Translate
		(
			Random(0.0f, mWorldWidth),
			Random(0.0f, mWorldHeight),
			0.0f
		);

		mComponents.transforms.GetComponent(mTarget2)->Translate
		(
			Random(0.0f, mWorldWidth),
			Random(0.0f, mWorldHeight),
			0.0f
		);

		mComponents.steering.GetComponent(mAvoider1)->target = mTarget1;
		mComponents.steering.GetComponent(mAvoider2)->target = mTarget2;
	}
	flip = !flip;
		}, true);
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
	auto drawSphere = [&](Entity entity, float radius, XMVECTOR color = Colors::White)
	{
		Debug::Sphere(mComponents.transforms.GetComponent(entity)->WorldPosition(),
			radius, mView, mProj, graphics, color);
	};

	auto drawCapsule = [&](Entity entity, float radius, float halfHeight, XMVECTOR color = Colors::White)
	{
		EntityTransform& transform = *mComponents.transforms.GetComponent(entity);
		Debug::Capsule(transform.WorldPosition(), transform.WorldForward(),
			radius, halfHeight, mView, mProj, graphics, color);
	};

	//drawSphere(mSeeker, r);
	//drawSphere(mArriver, r, Colors::PowderBlue);
	//drawSphere(mRandomSeeker, r, Colors::MediumAquamarine);
	//drawSphere(mRandomTarget, r, Colors::MediumOrchid);
	//drawSphere(mWanderer, r, Colors::MediumPurple);

	drawSphere(mTarget1, r);
	drawSphere(mTarget2, r);
	drawSphere(mAvoider1, r, Colors::Black);
	drawSphere(mAvoider2, r, Colors::Black);

	Entity child1 = *mComponents.hierarchies.GetComponent(mAvoider1)->children.begin();
	Entity child2 = *mComponents.hierarchies.GetComponent(mAvoider2)->children.begin();
	Collider& avoidCollider1 = *mComponents.colliders.GetComponent(child1);
	Collider& avoidCollider2 = *mComponents.colliders.GetComponent(child2);
	drawCapsule(child1, avoidCollider1.r, avoidCollider1.hh, Colors::Gray);
	drawCapsule(child2, avoidCollider2.r, avoidCollider2.hh, Colors::Gray);
}

// Wandering behaviour test:
//Vector3 wandererPosition = mComponents.transforms.GetComponent(mWanderer)->Translation();
//if (wandererPosition.x > mWorldWidth || wandererPosition.x < 0.0f ||
//	wandererPosition.y > mWorldHeight || wandererPosition.y < 0.0f)
//{
//	mComponents.transforms.GetComponent(mWanderer)->Translate(mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f);
//}
