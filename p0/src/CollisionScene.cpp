#include "pch.h"
#include "CollisionScene.h"
#include "DebugRenderer.h"

namespace
{
	constexpr float r = 25.0f;
	constexpr float hh = 75.0f;
}

using namespace DirectX;

CollisionScene::CollisionScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) :
	Scene(graphics, audio)
{
	mSS.a = { -500.0f, 0.0f, 0.0f };

	mCC.a.Translate(500.0f, 0.0f, 0.0f);
	mCC.b.RotateZ(90.0f);

	mSC.a = { 0.0f, 250.0f, 0.0f };
	mSC.b.Translate(0.0f, 250.0f, 0.0f);
	mSC.b.RotateZ(90.0f);
	
	mSoccer.player.RotateZ(-45.0f);
	mSoccer.player.Translate(-500.0f, -500.0f, 0.0f);

	mRange.target = { 0.0f, -500.0f, 0.0f };
	mRange.viewer.Translate(0.0f, -400.0f, 0.0f);
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

void CollisionScene::OnUpdate(float dt, float tt, DX::Input& input)
{
	const float speed = 100.0f * dt;
	Vector3 mtv;

	mSS.b = mSS.a;
	mSS.b += { r* cos(tt), r* sin(tt), 0.0f };
	mSS.b += SphereSphere(mSS.a, mSS.b, r, r, mtv) ? mtv : Vector3::Zero;
	mSS.color = SphereSphere(mSS.a, mSS.b, r, r) ? Colors::Red : Colors::Green;

	mCC.b.Translate(Vector3{ mCC.a.Translation().x, hh + r, 0.0f } + Vector3{ cos(tt) * speed, 0.0f, 0.0f });
	mCC.b.DeltaRotateZ(speed);
	mCC.b.DeltaTranslate(CapsuleCapsule(mCC.a, mCC.b, r, r, hh, hh, mtv) ? mtv : Vector3::Zero);
	mCC.color = CapsuleCapsule(mCC.a, mCC.b, r, r, hh, hh) ? Colors::Red : Colors::Green;

	mSC.b.Translate(mSC.a + Vector3{ cos(tt) * hh, -r, 0.0f });
	mSC.b.DeltaTranslate(SphereCapsule(mSC.a, mSC.b, r, r, hh, mtv) ? mtv : Vector3::Zero);
	mSC.color = SphereCapsule(mSC.a, mSC.b, r, r, hh) ? Colors::Red : Colors::Green;

	mSoccer.player.DeltaTranslate(mSoccer.player.Forward() * speed);
	mSoccer.ball += SphereCapsule(mSoccer.ball, mSoccer.player, r, r, hh, mtv) ? -mtv : Vector3::Zero;
	mSoccer.color = SphereCapsule(mSoccer.ball, mSoccer.player, r, r, hh) ? Colors::Red : Colors::Green;
	if (mSoccer.player.Translation().y > 500.0f)
	{
		mSoccer.player.Translate(-500.0f, -500.0f, 0.0f);
		mSoccer.ball = Vector3::Zero;
	}

	mRange.viewer.RotateZ(tt * 100.0f);
}

void CollisionScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	Vector3 direction = mSS.b - mSS.a;
	direction.Normalize();
	Debug::Sphere(mSS.a + r * 0.5f * direction, r * 0.5f, mView, mProj, graphics);
	Debug::Sphere(mSS.b - r * 0.5f * direction, r * 0.5f, mView, mProj, graphics);
	Debug::Sphere(mSS.a, r, mView, mProj, graphics, mSS.color, true);
	Debug::Sphere(mSS.b, r, mView, mProj, graphics, mSS.color, true);

	Vector3 aNearest, bNearest;
	NearestCylinderPoints(mCC.a, mCC.b, hh, hh, aNearest, bNearest);
	Debug::Sphere(aNearest, r, mView, mProj, graphics, Colors::Black);
	Debug::Sphere(bNearest, r, mView, mProj, graphics, Colors::White);
	Debug::Capsule(mCC.a, r, hh, mView, mProj, graphics, mCC.color, true);
	Debug::Capsule(mCC.b, r, hh, mView, mProj, graphics, mCC.color, true);

	Debug::Sphere(mSC.a, r, mView, mProj, graphics, mSC.color, true);
	Debug::Capsule(mSC.b, r, hh, mView, mProj, graphics, mSC.color, true);

	Debug::Capsule(mSoccer.player, r, hh, mView, mProj, graphics, mSoccer.color, true);
	Debug::Sphere(mSoccer.ball, r, mView, mProj, graphics, mSoccer.color, true);

	//Debug::Capsule(mRange.viewer, r, hh, mView, mProj, graphics);
	Debug::InRange(mRange.viewer, mRange.target, mRange.length, mRange.fov, mView, mProj, graphics);
	Debug::Sphere(mRange.target, r, mView, mProj, graphics);
}
