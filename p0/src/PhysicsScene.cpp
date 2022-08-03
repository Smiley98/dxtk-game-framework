#include "pch.h"
#include "PhysicsScene.h"
#include "DebugRenderer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PhysicsScene::PhysicsScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	auto context = graphics->GetD3DDeviceContext();
	auto device = graphics->GetD3DDevice();
	mStates = std::make_unique<CommonStates>(device);

	mSphereSphereA.translation = { -500.0f, 0.0f, 0.0f };
	mSphereSphereA.radius = mRadius;
	mSphereSphereB = mSphereSphereA;

	mCapsuleCapsuleA.transform.Translate({ 0.0, 0.0f, 0.0f });
	mCapsuleCapsuleA.halfHeight = mHalfHeight;
	mCapsuleCapsuleA.radius = mRadius;
	mCapsuleCapsuleB = mCapsuleCapsuleA;
	mCapsuleCapsuleB.transform.Translate({ -mHalfHeight, 0.0f, 0.0f });
	mCapsuleCapsuleB.transform.Rotate({ 0.0f, 0.0f, 45.0f });
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float width = float(size.right - size.left);
	const float height = float(size.bottom - size.top);
	mProjection = Matrix::CreateOrthographic(width, height, 0.01f, 1000.0f);
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

	mSphereSphereB.translation = mSphereSphereA.translation;
	mSphereSphereB.translation += { mRadius* cos(tt), mRadius* sin(tt), 0.0f };
	Vector3 mtv;
	if (mSphereSphereB.IsColliding(mSphereSphereA, mtv))
		mSphereSphereB.translation += mtv;
	mSphereSphereColor = mSphereSphereB.IsColliding(mSphereSphereA) ? Colors::Red : Colors::Green;

	const float distance = 100.0f;
	const float height = mHalfHeight * 2.0f;
	//mCapsuleCapsuleB.transform.Translate(mCapsuleCapsuleA.transform.Translation() + Vector3{ cos(tt) * distance, 0.0f, 0.0f });
	//mCapsuleCapsuleB.transform.DeltaRotate({ 0.0f, 0.0f, speed * 0.1f });
	mCapsuleCapsuleColor = mCapsuleCapsuleB.IsColliding(mCapsuleCapsuleA) ? Colors::Red : Colors::Green;
}

void PhysicsScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	Debug::Draw(mSphereSphereA, mView, mProjection, graphics, mSphereSphereColor);
	Debug::Draw(mSphereSphereB, mView, mProjection, graphics, mSphereSphereColor);

	Debug::Draw(mCapsuleCapsuleA, mView, mProjection, graphics, mCapsuleCapsuleColor, true);
	Debug::Draw(mCapsuleCapsuleB, mView, mProjection, graphics, mCapsuleCapsuleColor, true);

	Vector3 bUpper, bLower, aNearest, bNearest;
	Bounds(mCapsuleCapsuleB.transform, mCapsuleCapsuleB.halfHeight, bUpper, bLower);
	NearestSpheres(mCapsuleCapsuleA.transform, mCapsuleCapsuleA.halfHeight, mCapsuleCapsuleA.radius, mCapsuleCapsuleB.transform, mCapsuleCapsuleB.halfHeight, mCapsuleCapsuleB.radius, aNearest, bNearest);
	Debug::Draw({ aNearest, mRadius }, mView, mProjection, graphics);
	Debug::Draw({ bNearest, mRadius }, mView, mProjection, graphics, Colors::Aqua);
	//Debug::Draw({ bUpper, mRadius }, mView, mProjection, graphics, Colors::Purple);
	//Debug::Draw({ bLower, mRadius }, mView, mProjection, graphics, Colors::Blue);
}
