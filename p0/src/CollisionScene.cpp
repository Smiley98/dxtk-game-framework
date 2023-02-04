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

CollisionScene::CollisionScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) :
	Scene(graphics, audio)
{
	auto createSphere = [&](float x, float y) -> Entity
	{
		Entity entity = CreateEntity(sComponents, x, y);
		AddSphere(entity, r, sComponents);
		return entity;
	};

	auto createCapsule = [&](float x, float y) -> Entity
	{
		Entity entity = CreateEntity(sComponents, x, y);
		AddCapsule(entity, r, hh, sComponents);
		return entity;
	};

	mSS.a = createSphere(-500.0f, 0.0f);
	mSS.b = createSphere(-500.0f, 0.0f);

	mCC.a = createCapsule(500.0f, 0.0f);
	mCC.b = createCapsule(0.0f, 0.0f);
	sComponents.GetTransform(mCC.b).RotateZ(90.0f);

	mSC.a = createSphere(0.0f, 250.0f);
	mSC.b = createCapsule(0.0f, 250.0f);
	sComponents.GetTransform(mSC.b).RotateZ(90.0f);
	
	mSoccer.player = createCapsule(-500.0f, -500.0f);
	mSoccer.ball = createSphere(0.0f, 0.0f);
	sComponents.GetTransform(mSoccer.player).RotateZ(-45.0f);

	mRange.viewer = CreateEntity(sComponents, 0.0f, -400.0f);
	mRange.target = CreateEntity(sComponents, 0.0f, -500.0f);
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

void CollisionScene::OnUpdate(float dt, float tt)
{
	const float speed = 100.0f * dt;
	Vector3 mtv;

	// Sphere-Sphere
	sComponents.GetTransform(mSS.b).Translate(sComponents.GetTransform(mSS.a).Translation());
	sComponents.GetTransform(mSS.b).DeltaTranslate(r* cos(tt), r* sin(tt), 0.0f);

	sComponents.GetTransform(mSS.b).DeltaTranslate(
		Collision::IsColliding(mSS.a, mSS.b, mtv, sComponents) ? mtv : Vector3::Zero);
	mSS.color = Collision::IsColliding(mSS.a, mSS.b, mtv, sComponents) ? Colors::Red : Colors::Green;

	// Capsule-Capsule
	sComponents.GetTransform(mCC.b).Translate(
		Vector3{ sComponents.GetTransform(mCC.a).Translation().x, hh + r, 0.0f } +
		Vector3{ cos(tt) * speed, 0.0f, 0.0f });
	sComponents.GetTransform(mCC.b).DeltaRotateZ(speed);

	sComponents.GetTransform(mCC.b).DeltaTranslate(
		Collision::IsColliding(mCC.a, mCC.b, mtv, sComponents) ? mtv : Vector3::Zero);
	mCC.color = Collision::IsColliding(mCC.a, mCC.b, mtv, sComponents) ? Colors::Red : Colors::Green;

	// Sphere-Capsule
	sComponents.GetTransform(mSC.b).Translate(sComponents.GetTransform(mSC.a).Translation() + Vector3{ cos(tt) * hh, -r, 0.0f });

	sComponents.GetTransform(mSC.b).DeltaTranslate(
		Collision::IsColliding(mSC.a, mSC.b, mtv, sComponents) ? mtv : Vector3::Zero);
	mSC.color = Collision::IsColliding(mSC.a, mSC.b, mtv, sComponents) ? Colors::Red : Colors::Green;

	// Capsule-Sphere (soccer)
	EntityTransform& tPlayer = sComponents.GetTransform(mSoccer.player);
	EntityTransform& tBall = sComponents.GetTransform(mSoccer.ball);

	tPlayer.DeltaTranslate(tPlayer.Forward() * speed);
	tBall.DeltaTranslate(Collision::IsColliding(mSoccer.player, mSoccer.ball, mtv, sComponents) ?
		mtv : Vector3::Zero);
	mSoccer.color = Collision::IsColliding(mSoccer.ball, mSoccer.player, mtv, sComponents) ?
		Colors::Red : Colors::Green;

	if (tPlayer.Translation().y > 500.0f)
	{
		tPlayer.Translate(-500.0f, -500.0f, 0.0f);
		tBall.Translate(Vector3::Zero);
	}

	sComponents.GetTransform(mRange.viewer).RotateZ(tt * 100.0f);
}

void CollisionScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	{
		EntityTransform& tA = sComponents.GetTransform(mSS.a);
		EntityTransform& tB = sComponents.GetTransform(mSS.b);
		Vector3 direction = tB.WorldPosition() - tA.WorldPosition();
		direction.Normalize();

		Debug::DrawSphere(tA.WorldPosition() + r * 0.5f * direction, r * 0.5f);
		Debug::DrawSphere(tB.WorldPosition() - r * 0.5f * direction, r * 0.5f);
		Debug::DrawSphere(tA.WorldPosition(), r, mSS.color, true);
		Debug::DrawSphere(tB.WorldPosition(), r, mSS.color, true);
	}
	
	{
		EntityTransform& tA = sComponents.GetTransform(mCC.a);
		EntityTransform& tB = sComponents.GetTransform(mCC.b);
		Vector3 aNearest, bNearest;
		NearestCylinderPoints(
			tA.WorldPosition(), tB.WorldPosition(),
			tA.WorldForward(), tB.WorldForward(),
		hh, hh, aNearest, bNearest);

		Debug::DrawSphere(aNearest, r, Colors::Black);
		Debug::DrawSphere(bNearest, r, Colors::White);
		Debug::DrawCapsule(tA.WorldPosition(), tA.WorldForward(), r, hh, mCC.color, true);
		Debug::DrawCapsule(tB.WorldPosition(), tB.WorldForward(), r, hh, mCC.color, true);
	}

	{
		EntityTransform& tA = sComponents.GetTransform(mSC.a);
		EntityTransform& tB = sComponents.GetTransform(mSC.b);
		Debug::DrawSphere(tA.WorldPosition(), r, mSC.color, true);
		Debug::DrawCapsule(tB.WorldPosition(), tB.WorldForward(), r, hh, mSC.color, true);
	}
	
	{
		EntityTransform& tPlayer = sComponents.GetTransform(mSoccer.player);
		EntityTransform& tBall = sComponents.GetTransform(mSoccer.ball);
		Debug::DrawCapsule(tPlayer.WorldPosition(), tPlayer.WorldForward(), r, hh, mSoccer.color, true);
		Debug::DrawSphere(tBall.WorldPosition(), r, mSoccer.color, true);
	}

	{
		EntityTransform& tViewer = sComponents.GetTransform(mRange.viewer);
		EntityTransform& tTarget = sComponents.GetTransform(mRange.target);
		Debug::DrawFoV(tViewer.WorldPosition(), tViewer.WorldForward(), tTarget.WorldPosition(), mRange.length, mRange.fov);
		Debug::DrawSphere(tTarget.WorldPosition(), r);
	}
}
