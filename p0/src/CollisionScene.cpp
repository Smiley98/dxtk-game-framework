#include "pch.h"
#include "CollisionScene.h"
#include "CollisionMath.h"
#include "Collider.h"
#include "CollisionSystem.h"
#include "DebugRenderer.h"
#include "Utility.h"

namespace
{
	constexpr float r = 25.0f;
	constexpr float hh = 75.0f;
}

using namespace DirectX;

CollisionScene::CollisionScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components) :
	Scene(graphics, audio, components)
{
	auto createSphere = [&](float x, float y) -> Entity
	{
		Entity entity = CreateEntity(mComponents, x, y);
		AddSphere(entity, r, mComponents);
		return entity;
	};

	auto createCapsule = [&](float x, float y) -> Entity
	{
		Entity entity = CreateEntity(mComponents, x, y);
		AddCapsule(entity, r, hh, mComponents);
		return entity;
	};

	mSS.a = createSphere(-500.0f, 0.0f);
	mSS.b = createSphere(-500.0f, 0.0f);

	mCC.a = createCapsule(500.0f, 0.0f);
	mCC.b = createCapsule(0.0f, 0.0f);
	components.transforms.GetComponent(mCC.b)->RotateZ(90.0f);

	mSC.a = createSphere(0.0f, 250.0f);
	mSC.b = createCapsule(0.0f, 250.0f);
	components.transforms.GetComponent(mSC.b)->RotateZ(90.0f);
	
	mSoccer.player = createCapsule(-500.0f, -500.0f);
	mSoccer.ball = createSphere(0.0f, 0.0f);
	components.transforms.GetComponent(mSoccer.player)->RotateZ(-45.0f);

	mRange.viewer = CreateEntity(components, 0.0f, -400.0f);
	mRange.target = CreateEntity(components, 0.0f, -500.0f);
}

CollisionScene::~CollisionScene()
{
}

void CollisionScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float width = float(size.right - size.left);
	const float height = float(size.bottom - size.top);
	mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 100.0f }, {}, Vector3::UnitY);
	mProj = Matrix::CreateOrthographic(width, height, 0.01f, 1000.0f);
}

void CollisionScene::OnBegin()
{
}

void CollisionScene::OnEnd()
{
}

void CollisionScene::OnPause()
{
}

void CollisionScene::OnResume()
{
}

void CollisionScene::OnUpdate(float dt, float tt, const DX::Input& input)
{
	const float speed = 100.0f * dt;
	Vector3 mtv;

	// Sphere-Sphere
	mComponents.transforms.GetComponent(mSS.b)->Translate(
		mComponents.transforms.GetComponent(mSS.a)->Translation());
	mComponents.transforms.GetComponent(mSS.b)->DeltaTranslate(r* cos(tt), r* sin(tt), 0.0f);

	mComponents.transforms.GetComponent(mSS.b)->DeltaTranslate(
		Collision::IsColliding(mSS.a, mSS.b, mtv, mComponents) ? mtv : Vector3::Zero);
	mSS.color = Collision::IsColliding(mSS.a, mSS.b, mtv, mComponents) ? Colors::Red : Colors::Green;

	// Capsule-Capsule
	mComponents.transforms.GetComponent(mCC.b)->Translate(
		Vector3{ mComponents.transforms.GetComponent(mCC.a)->Translation().x, hh + r, 0.0f } +
		Vector3{ cos(tt) * speed, 0.0f, 0.0f });
	mComponents.transforms.GetComponent(mCC.b)->DeltaRotateZ(speed);

	mComponents.transforms.GetComponent(mCC.b)->DeltaTranslate(
		Collision::IsColliding(mCC.a, mCC.b, mtv, mComponents) ? mtv : Vector3::Zero);
	mCC.color = Collision::IsColliding(mCC.a, mCC.b, mtv, mComponents) ? Colors::Red : Colors::Green;

	// Sphere-Capsule
	mComponents.transforms.GetComponent(mSC.b)->Translate(
		mComponents.transforms.GetComponent(mSC.a)->Translation() + Vector3{ cos(tt) * hh, -r, 0.0f });

	mComponents.transforms.GetComponent(mSC.b)->DeltaTranslate(
		Collision::IsColliding(mSC.a, mSC.b, mtv, mComponents) ? mtv : Vector3::Zero);
	mSC.color = Collision::IsColliding(mSC.a, mSC.b, mtv, mComponents) ? Colors::Red : Colors::Green;

	// Capsule-Sphere (soccer)
	EntityTransform& tPlayer = *mComponents.transforms.GetComponent(mSoccer.player);
	EntityTransform& tBall = *mComponents.transforms.GetComponent(mSoccer.ball);

	tPlayer.DeltaTranslate(tPlayer.Forward() * speed);
	tBall.DeltaTranslate(Collision::IsColliding(mSoccer.player, mSoccer.ball, mtv, mComponents) ?
		mtv : Vector3::Zero);
	mSoccer.color = Collision::IsColliding(mSoccer.ball, mSoccer.player, mtv, mComponents) ?
		Colors::Red : Colors::Green;

	if (tPlayer.Translation().y > 500.0f)
	{
		tPlayer.Translate(-500.0f, -500.0f, 0.0f);
		tBall.Translate(Vector3::Zero);
	}

	mComponents.transforms.GetComponent(mRange.viewer)->RotateZ(tt * 100.0f);
}

void CollisionScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	{
		EntityTransform& tA = *mComponents.transforms.GetComponent(mSS.a);
		EntityTransform& tB = *mComponents.transforms.GetComponent(mSS.b);
		Vector3 direction = tB.WorldPosition() - tA.WorldPosition();
		direction.Normalize();

		Debug::Sphere(tA.WorldPosition() + r * 0.5f * direction, r * 0.5f, mView, mProj, graphics);
		Debug::Sphere(tB.WorldPosition() - r * 0.5f * direction, r * 0.5f, mView, mProj, graphics);
		Debug::Sphere(tA.WorldPosition(), r, mView, mProj, graphics, mSS.color, true);
		Debug::Sphere(tB.WorldPosition(), r, mView, mProj, graphics, mSS.color, true);
	}
	
	{
		EntityTransform& tA = *mComponents.transforms.GetComponent(mCC.a);
		EntityTransform& tB = *mComponents.transforms.GetComponent(mCC.b);
		Vector3 aNearest, bNearest;
		NearestCylinderPoints(
			tA.WorldPosition(), tB.WorldPosition(),
			tA.WorldForward(), tB.WorldForward(),
		hh, hh, aNearest, bNearest);

		Debug::Sphere(aNearest, r, mView, mProj, graphics, Colors::Black);
		Debug::Sphere(bNearest, r, mView, mProj, graphics, Colors::White);
		Debug::Capsule(tA.WorldPosition(), tA.WorldForward(), r, hh, mView, mProj, graphics, mCC.color, true);
		Debug::Capsule(tB.WorldPosition(), tB.WorldForward(), r, hh, mView, mProj, graphics, mCC.color, true);
	}

	{
		EntityTransform& tA = *mComponents.transforms.GetComponent(mSC.a);
		EntityTransform& tB = *mComponents.transforms.GetComponent(mSC.b);
		Debug::Sphere(tA.WorldPosition(), r, mView, mProj, graphics, mSC.color, true);
		Debug::Capsule(tB.WorldPosition(), tB.WorldForward(), r, hh, mView, mProj, graphics, mSC.color, true);
	}
	
	{
		EntityTransform& tPlayer = *mComponents.transforms.GetComponent(mSoccer.player);
		EntityTransform& tBall = *mComponents.transforms.GetComponent(mSoccer.ball);
		Debug::Capsule(tPlayer.WorldPosition(), tPlayer.WorldForward(), r, hh, mView, mProj, graphics, mSoccer.color, true);
		Debug::Sphere(tBall.WorldPosition(), r, mView, mProj, graphics, mSoccer.color, true);
	}

	{
		EntityTransform& tViewer = *mComponents.transforms.GetComponent(mRange.viewer);
		EntityTransform& tTarget = *mComponents.transforms.GetComponent(mRange.target);
		Debug::InRange(tViewer.WorldPosition(), tViewer.WorldForward(), tTarget.WorldPosition(),
			mRange.length, mRange.fov, mView, mProj, graphics);
		Debug::Sphere(tTarget.WorldPosition(), r, mView, mProj, graphics);
	}
}
