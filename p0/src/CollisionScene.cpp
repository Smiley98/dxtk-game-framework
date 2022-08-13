#include "pch.h"
#include "CollisionScene.h"
#include "DebugRenderer.h"

namespace
{
	constexpr float r = 25.0f;
	constexpr float hh = 75.0f;
}

using namespace DirectX;
using namespace Collision;

CollisionScene::CollisionScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) :
	Scene(graphics, audio),
	mSphereSphereA(r),
	mSphereSphereB(r),
	mCapsuleCapsuleA(hh, r),
	mCapsuleCapsuleB(hh, r),
	mSphere1(r),
	mSphere2(r),
	mCapsule1(hh, r),
	mCapsule2(hh, r)
{
	auto context = graphics->GetD3DDeviceContext();
	auto device = graphics->GetD3DDevice();

	mSphereSphereA.position = { -500.0f, 0.0f, 0.0f };

	mCapsuleCapsuleA.Translate({ 500.0, 0.0f, 0.0f });
	mCapsuleCapsuleB.DeltaRotate(90.0f);
	
	mCapsule1.Translate({ 0.0f, 250.0f, 0.0f });
	mCapsule1.DeltaRotate(90.0f);
	mSphere1.position = { 0.0f, 250.0f, 0.0f };
	
	mCapsule2.DeltaRotate(-45.0f);
	mCapsule2.Translate({ -500, -500, 0.0f });
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
		mSphereSphereB.position = mSphereSphereA.position;
		mSphereSphereB.position += { r * cos(tt), r * sin(tt), 0.0f };
		Vector3 mtv;
		if (mSphereSphereB.IsColliding(mSphereSphereA, mtv))
			mSphereSphereB.position += mtv;
		mSphereSphereColor = mSphereSphereB.IsColliding(mSphereSphereA) ? Colors::Red : Colors::Green;
	}

	{
		const Vector3 origin{ mCapsuleCapsuleA.Translation().x, hh + r, 0.0f };
		mCapsuleCapsuleB.Translate(origin + Vector3 { cos(tt) * speed, 0.0f, 0.0f });
		mCapsuleCapsuleB.DeltaRotate({ 0.0f, 0.0f, speed });

		Vector3 mtv;
		if (mCapsuleCapsuleB.IsColliding(mCapsuleCapsuleA, mtv))
			mCapsuleCapsuleB.DeltaTranslate(mtv);
		mCapsuleCapsuleColor = mCapsuleCapsuleB.IsColliding(mCapsuleCapsuleA) ? Colors::Red : Colors::Green;
	}

	{
		// Capsule-Sphere
		Vector3 mtv;
		mCapsule1.Translate(mSphere1.position + Vector3{ cos(tt) * hh, -r, 0.0f });
		if (mCapsule1.IsColliding(mSphere1, mtv))
			mCapsule1.DeltaTranslate(mtv);
		mColor1 = mCapsule1.IsColliding(mSphere1) ? Colors::Red : Colors::Green;

		// Sphere-Capsule
		//mSphere1.g.t = mCapsule1.g.t.Translation() + Vector3{ cos(tt) * mHalfHeight, -mRadius, 0.0f };
		//if (mSphere1.IsColliding(mCapsule1, mtv))
		//	mSphere1.g.t += mtv;
		//mColor1 = mSphere1.IsColliding(mCapsule1) ? Colors::Red : Colors::Green;
	}

	{
		// Soccer (Sphere-Capsule)
		mCapsule2.DeltaTranslate(mCapsule2.Forward() * speed);
		Vector3 mtv;
		if (mSphere2.IsColliding(mCapsule2, mtv))
			mSphere2.position += mtv;
		mColor2 = mSphere2.IsColliding(mCapsule2) ? Colors::Red : Colors::Green;

		if (mCapsule2.Translation().y > 500.0f)
		{
			mCapsule2.Translate({ -500, -500, 0.0f });
			mSphere2.position = Vector3::Zero;
		}
	}
}

void CollisionScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	Debug::Draw(mSphereSphereA, mView, mProj, graphics, mSphereSphereColor, true);
	Debug::Draw(mSphereSphereB, mView, mProj, graphics, mSphereSphereColor, true);
	DebugSpheres(mSphereSphereA, mSphereSphereB, graphics);

	Debug::Draw(mCapsuleCapsuleA, mView, mProj, graphics, mCapsuleCapsuleColor, true);
	Debug::Draw(mCapsuleCapsuleB, mView, mProj, graphics, mCapsuleCapsuleColor, true);
	DebugCapsules(mCapsuleCapsuleA, mCapsuleCapsuleB, graphics);

	Debug::Draw(mSphere1, mView, mProj, graphics, mColor1, true);
	Debug::Draw(mCapsule1, mView, mProj, graphics, mColor1, true);

	Debug::Draw(mSphere2, mView, mProj, graphics, mColor2, true);
	Debug::Draw(mCapsule2, mView, mProj, graphics, mColor2, true);
}

void CollisionScene::DebugSpheres(const Collision::SphereCollider& a, const Collision::SphereCollider& b, std::shared_ptr<DX::DeviceResources> graphics)
{
	Vector3 direction = b.position - a.position;
	direction.Normalize();

	SphereCollider halfA(a.Radius() * 0.5f);
	SphereCollider halfB(b.Radius() * 0.5f);
	halfA.position = a.position + halfA.Radius() * direction;
	halfB.position = b.position + halfB.Radius() * -direction;
	Debug::Draw(halfA, mView, mProj, graphics);
	Debug::Draw(halfB, mView, mProj, graphics);
}

void CollisionScene::DebugCapsules(const Collision::CapsuleCollider& a, const Collision::CapsuleCollider& b, std::shared_ptr<DX::DeviceResources> graphics)
{
	Vector3 aNearest, bNearest;
	NearestSpheres(a, b, a.HalfHeight(), b.HalfHeight(), a.Radius(), b.Radius(), aNearest, bNearest);
	SphereCollider aSphere(a.Radius());
	SphereCollider bSphere(a.Radius());
	aSphere.position = aNearest;
	bSphere.position = bNearest;
	Debug::Draw(aSphere, mView, mProj, graphics, Colors::Black);
	Debug::Draw(bSphere, mView, mProj, graphics, Colors::White);
}
