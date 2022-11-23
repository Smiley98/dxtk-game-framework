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
	auto context = graphics->GetD3DDeviceContext();
	auto device = graphics->GetD3DDevice();

	mSS.gA.r = r;
	mSS.gB.r = r;
	mCC.gA.r = r;
	mCC.gB.r = r;
	mSC.gA.r = r;
	mSC.gB.r = r;
	mCC.gA.hh = hh;
	mCC.gB.hh = hh;
	mSC.gB.hh = hh;

	mSoccer.gPlayer.hh = hh;
	mSoccer.gPlayer.r = r;
	mSoccer.gBall.r = r;

	mSS.tA.Translate(-500.0f, 0.0f, 0.0f);

	mCC.tA.Translate(500.0f, 0.0f, 0.0f);
	mCC.tB.RotateZ(90.0f);

	mSC.tA.Translate(0.0f, 250.0f, 0.0f);
	mSC.tB.Translate(0.0f, 250.0f, 0.0f);
	mSC.tB.RotateZ(90.0f);
	
	mSoccer.tPlayer.RotateZ(-45.0f);
	mSoccer.tPlayer.Translate(-500.0f, -500.0f, 0.0f);
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

	mSS.tB.Translate(mSS.tA.Translation());
	mSS.tB.DeltaTranslate(r * cos(tt), r * sin(tt), 0.0f);
	mSS.tB.DeltaTranslate(SphereSphere(mSS.tA, mSS.tB, mSS.gA, mSS.gB, mtv) ? mtv : Vector3::Zero);
	mSS.color = SphereSphere(mSS.tA, mSS.tB, mSS.gA, mSS.gB) ? Colors::Red : Colors::Green;

	mCC.tB.Translate(Vector3{ mCC.tA.Translation().x, hh + r, 0.0f } + Vector3{ cos(tt) * speed, 0.0f, 0.0f });
	mCC.tB.DeltaRotateZ(speed);
	mCC.tB.DeltaTranslate(CapsuleCapsule(mCC.tA, mCC.tB, mCC.gA, mCC.gB, mtv) ? mtv : Vector3::Zero);
	mCC.color = CapsuleCapsule(mCC.tA, mCC.tB, mCC.gA, mCC.gB) ? Colors::Red : Colors::Green;

	mSC.tB.Translate(mSC.tA.Translation() + Vector3{ cos(tt) * hh, -r, 0.0f });
	mSC.tB.DeltaTranslate(SphereCapsule(mSC.tA, mSC.tB, mSC.gA, mSC.gB, mtv) ? mtv : Vector3::Zero);
	mSC.color = SphereCapsule(mSC.tA, mSC.tB, mSC.gA, mSC.gB) ? Colors::Red : Colors::Green;

	mSoccer.tPlayer.DeltaTranslate(mSoccer.tPlayer.Forward() * speed);
	mSoccer.tBall.DeltaTranslate(SphereCapsule(mSoccer.tBall, mSoccer.tPlayer, mSoccer.gBall, mSoccer.gPlayer, mtv) ?
		-mtv : Vector3::Zero);
	mSoccer.color = SphereCapsule(mSoccer.tBall, mSoccer.tPlayer, mSoccer.gBall, mSoccer.gPlayer) ?
		Colors::Red : Colors::Green;
	if (mSoccer.tPlayer.Translation().y > 500.0f)
	{
		mSoccer.tPlayer.Translate(-500.0f, -500.0f, 0.0f);
		mSoccer.tBall.Translate(Vector3::Zero);
	}
}

void CollisionScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	Vector3 direction = mSS.tB.Translation() - mSS.tA.Translation();
	direction.Normalize();
	Debug::Draw(mSS.tA.Translation() + mSS.gA.r * 0.5f * direction, mSS.gA.r * 0.5f, mView, mProj, graphics);
	Debug::Draw(mSS.tB.Translation() - mSS.gB.r * 0.5f * direction, mSS.gA.r * 0.5f, mView, mProj, graphics);
	Debug::Draw(mSS.tA, mSS.gA, mView, mProj, graphics, mSS.color, true);
	Debug::Draw(mSS.tB, mSS.gB, mView, mProj, graphics, mSS.color, true);

	Vector3 aNearest, bNearest;
	NearestCylinderPoints(mCC.tA, mCC.tB, mCC.gA.hh, mCC.gB.hh, aNearest, bNearest);
	Debug::Draw(aNearest, mCC.gA.r, mView, mProj, graphics, Colors::Black);
	Debug::Draw(bNearest, mCC.gB.r, mView, mProj, graphics, Colors::White);
	Debug::Draw(mCC.tA, mCC.gA, mView, mProj, graphics, mCC.color, true);
	Debug::Draw(mCC.tB, mCC.gB, mView, mProj, graphics, mCC.color, true);

	Debug::Draw(mSC.tA, mSC.gA, mView, mProj, graphics, mSC.color, true);
	Debug::Draw(mSC.tB, mSC.gB, mView, mProj, graphics, mSC.color, true);

	Debug::Draw(mSoccer.tPlayer, mSoccer.gPlayer, mView, mProj, graphics, mSoccer.color, true);
	Debug::Draw(mSoccer.tBall, mSoccer.gBall, mView, mProj, graphics, mSoccer.color, true);
}
