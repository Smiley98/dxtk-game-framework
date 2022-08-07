#include "pch.h"
#include "CollisionScene.h"
#include "DebugRenderer.h"

using namespace DirectX;

CollisionScene::CollisionScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	auto context = graphics->GetD3DDeviceContext();
	auto device = graphics->GetD3DDevice();

	mSphereSphereA.g.t = { -500.0f, 0.0f, 0.0f };
	mSphereSphereA.g.r = mRadius;
	mSphereSphereB = mSphereSphereA;

	mCapsuleCapsuleA.g.t.Translate({ 500.0, 0.0f, 0.0f });
	mCapsuleCapsuleB.g.t.DeltaRotate(90.0f);
	mCapsuleCapsuleA.g.hh = mHalfHeight;
	mCapsuleCapsuleB.g.hh = mHalfHeight;
	mCapsuleCapsuleA.g.r = mRadius;
	mCapsuleCapsuleB.g.r = mRadius;

	mCapsule1.g.t.Translate({ 0.0f, 250.0f, 0.0f });
	mCapsule1.g.t.DeltaRotate(90.0f);
	mCapsule1.g.hh = mHalfHeight;
	mCapsule1.g.r = mRadius;
	mSphere1.g.t ={ 0.0f, 250.0f, 0.0f };
	mSphere1.g.r = mRadius;

	mCapsule2.g.t.DeltaRotate(-45.0f);
	mCapsule2.g.t.Translate({ -500, -500, 0.0f });
	mCapsule2.g.hh = mHalfHeight;
	mCapsule2.g.r = mRadius;
	mSphere2.g.r = mRadius;
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

void CollisionScene::OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse)
{
	const float dt = (float)timer.GetElapsedSeconds();
	const float tt = (float)timer.GetTotalSeconds();
	const float speed = 100.0f * dt;

	{
		mSphereSphereB.g.t = mSphereSphereA.g.t;
		mSphereSphereB.g.t += { mRadius* cos(tt), mRadius* sin(tt), 0.0f };
		Vector3 mtv;
		if (mSphereSphereB.IsColliding(mSphereSphereA, mtv))
			mSphereSphereB.g.t += mtv;
		mSphereSphereColor = mSphereSphereB.IsColliding(mSphereSphereA) ? Colors::Red : Colors::Green;
	}

	{
		const Vector3 origin{ 500.0f, mHalfHeight + mRadius, 0.0f };
		mCapsuleCapsuleB.g.t.Translate(origin + Vector3 { cos(tt) * speed, 0.0f, 0.0f });
		mCapsuleCapsuleB.g.t.DeltaRotate({ 0.0f, 0.0f, speed });

		Vector3 mtv;
		if (mCapsuleCapsuleB.IsColliding(mCapsuleCapsuleA, mtv))
			mCapsuleCapsuleB.g.t.DeltaTranslate(mtv);
		mCapsuleCapsuleColor = mCapsuleCapsuleB.IsColliding(mCapsuleCapsuleA) ? Colors::Red : Colors::Green;
	}

	{
		// Capsule-Sphere
		Vector3 mtv;
		mCapsule1.g.t.Translate(mSphere1.g.t + Vector3{ cos(tt) * mHalfHeight, -mRadius, 0.0f });
		if (mCapsule1.IsColliding(mSphere1, mtv))
			mCapsule1.g.t.DeltaTranslate(mtv);
		mColor1 = mCapsule1.IsColliding(mSphere1) ? Colors::Red : Colors::Green;

		// Sphere-Capsule
		//mSphere1.g.t = mCapsule1.g.t.Translation() + Vector3{ cos(tt) * mHalfHeight, -mRadius, 0.0f };
		//if (mSphere1.IsColliding(mCapsule1, mtv))
		//	mSphere1.g.t += mtv;
		//mColor1 = mSphere1.IsColliding(mCapsule1) ? Colors::Red : Colors::Green;
	}

	{
		// Soccer (Sphere-Capsule)
		mCapsule2.g.t.DeltaTranslate(mCapsule2.g.t.Forward() * speed);
		Vector3 mtv;
		if (mSphere2.IsColliding(mCapsule2, mtv))
			mSphere2.g.t += mtv;
		mColor2 = mSphere2.IsColliding(mCapsule2) ? Colors::Red : Colors::Green;

		if (mCapsule2.g.t.Translation().y > 500.0f)
		{
			mCapsule2.g.t.Translate({ -500, -500, 0.0f });
			mSphere2.g.t = Vector3::Zero;
		}
	}
}

void CollisionScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	Debug::Draw(mSphereSphereA.g, mView, mProj, graphics, mSphereSphereColor, true);
	Debug::Draw(mSphereSphereB.g, mView, mProj, graphics, mSphereSphereColor, true);
	DebugSpheres(mSphereSphereA.g, mSphereSphereB.g, graphics);

	Debug::Draw(mCapsuleCapsuleA.g, mView, mProj, graphics, mCapsuleCapsuleColor, true);
	Debug::Draw(mCapsuleCapsuleB.g, mView, mProj, graphics, mCapsuleCapsuleColor, true);
	DebugCapsules(mCapsuleCapsuleA.g, mCapsuleCapsuleB.g, graphics);

	Debug::Draw(mSphere1.g, mView, mProj, graphics, mColor1, true);
	Debug::Draw(mCapsule1.g, mView, mProj, graphics, mColor1, true);

	Debug::Draw(mSphere2.g, mView, mProj, graphics, mColor2, true);
	Debug::Draw(mCapsule2.g, mView, mProj, graphics, mColor2, true);
}

void CollisionScene::DebugSpheres(const Sphere& a, const Sphere& b, std::shared_ptr<DX::DeviceResources> graphics)
{
	Vector3 direction = b.t - a.t;
	direction.Normalize();
	Debug::Draw({ a.t + a.r *  direction * 0.5f, a.r * 0.5f }, mView, mProj, graphics);
	Debug::Draw({ b.t + b.r * -direction * 0.5f, b.r * 0.5f }, mView, mProj, graphics);
}

void CollisionScene::DebugCapsules(const Capsule& a, const Capsule& b, std::shared_ptr<DX::DeviceResources> graphics)
{
	Vector3 aNearest, bNearest;
	NearestSpheres(a, b, aNearest, bNearest);
	Debug::Draw({ aNearest, mRadius }, mView, mProj, graphics, Colors::Black);
	Debug::Draw({ bNearest, mRadius }, mView, mProj, graphics, Colors::White);
}
