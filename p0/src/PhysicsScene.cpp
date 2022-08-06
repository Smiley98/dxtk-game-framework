#include "pch.h"
#include "PhysicsScene.h"
#include "DebugRenderer.h"

using namespace DirectX;

PhysicsScene::PhysicsScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	auto context = graphics->GetD3DDeviceContext();
	auto device = graphics->GetD3DDevice();

	mSphereSphereA.g.t = { -500.0f, 0.0f, 0.0f };
	mSphereSphereA.g.r = mRadius;
	mSphereSphereB = mSphereSphereA;

	mCapsuleCapsuleA.g.t.Translate({ 500.0, 0.0f, 0.0f });
	mCapsuleCapsuleA.g.hh = mHalfHeight;
	mCapsuleCapsuleA.g.r = mRadius;
	mCapsuleCapsuleB = mCapsuleCapsuleA;
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float width = float(size.right - size.left);
	const float height = float(size.bottom - size.top);
	mProj = Matrix::CreateOrthographic(width, height, 0.01f, 1000.0f);
}

void PhysicsScene::OnBegin()
{
}

void PhysicsScene::OnEnd()
{
}

void PhysicsScene::OnPause()
{
}

void PhysicsScene::OnResume()
{
}

void PhysicsScene::OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse)
{
	mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 100.0f }, {}, Vector3::UnitY);
	const float dt = (float)timer.GetElapsedSeconds();
	const float tt = (float)timer.GetTotalSeconds();
	const float speed = 100.0f * dt;

	mSphereSphereB.g.t = mSphereSphereA.g.t;
	mSphereSphereB.g.t += { mRadius* cos(tt), mRadius* sin(tt), 0.0f };
	Vector3 mtv;
	if (mSphereSphereB.IsColliding(mSphereSphereA, mtv))
		mSphereSphereB.g.t += mtv;
	mSphereSphereColor = mSphereSphereB.IsColliding(mSphereSphereA) ? Colors::Red : Colors::Green;

	const float distance = 100.0f;
	const float height = mHalfHeight * 2.0f;
	mCapsuleCapsuleB.g.t.Translate(mCapsuleCapsuleA.g.t.Translation() + Vector3{ cos(tt) * distance, 0.0f, 0.0f });
	mCapsuleCapsuleB.g.t.DeltaRotate({ 0.0f, 0.0f, speed * 0.1f });
	mCapsuleCapsuleColor = mCapsuleCapsuleB.IsColliding(mCapsuleCapsuleA) ? Colors::Red : Colors::Green;
}

void PhysicsScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	Debug::Draw(mSphereSphereA.g, mView, mProj, graphics, mSphereSphereColor, true);
	Debug::Draw(mSphereSphereB.g, mView, mProj, graphics, mSphereSphereColor, true);
	DebugSpheres(mSphereSphereA.g, mSphereSphereB.g, graphics);

	Debug::Draw(mCapsuleCapsuleA.g, mView, mProj, graphics, mCapsuleCapsuleColor, true);
	Debug::Draw(mCapsuleCapsuleB.g, mView, mProj, graphics, mCapsuleCapsuleColor, true);
	DebugCapsules(mCapsuleCapsuleA.g, mCapsuleCapsuleB.g, graphics);
}

void PhysicsScene::DebugSpheres(const Sphere& a, const Sphere& b, std::shared_ptr<DX::DeviceResources> graphics)
{
	Vector3 direction = b.t - a.t;
	direction.Normalize();
	Debug::Draw({ a.t + a.r *  direction * 0.5f, a.r * 0.5f }, mView, mProj, graphics);
	Debug::Draw({ b.t + b.r * -direction * 0.5f, b.r * 0.5f }, mView, mProj, graphics);
}

void PhysicsScene::DebugCapsules(const Capsule& a, const Capsule& b, std::shared_ptr<DX::DeviceResources> graphics)
{
	Vector3 aUpper, aLower;
	Vector3 bUpper, bLower;
	Vector3 aNearest, bNearest;
	CylinderBounds(a, aUpper, aLower);
	CylinderBounds(b, bUpper, bLower);
	NearestSpheres(a, b, aNearest, bNearest);

	Debug::Draw({ aUpper, mRadius }, mView, mProj, graphics, Colors::Blue);
	Debug::Draw({ bUpper, mRadius }, mView, mProj, graphics, Colors::Blue);
	Debug::Draw({ aLower, mRadius }, mView, mProj, graphics, Colors::Purple);
	Debug::Draw({ bLower, mRadius }, mView, mProj, graphics, Colors::Purple);
	Debug::Draw({ aNearest, mRadius }, mView, mProj, graphics, Colors::Black);
	Debug::Draw({ bNearest, mRadius }, mView, mProj, graphics, Colors::White);
}
