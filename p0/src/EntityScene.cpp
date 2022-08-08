#include "pch.h"
#include "EntityScene.h"
#include "DebugRenderer.h"

using namespace DirectX;

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	mVan.Load(sPlayerRenderer, mCollision);
	mTd.Load(sBuildingRenderer, mCollision);

	mVan.transform.Translate({ -500.0f, -500.0f, 0.0f });
	mVan.transform.DeltaRotate(-45.0f);
	mVan.UpdateCollider(mCollision);
}

EntityScene::~EntityScene()
{
}

void EntityScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float width = float(size.right - size.left);
	const float height = float(size.bottom - size.top);
	const float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 60.0f * XM_PI / 180.0f;
	fovAngleY = aspectRatio < 1.0f ? fovAngleY * 2.0f : fovAngleY;
	mView = Matrix::CreateLookAt({ 0.0f, -100.0f, 1000.0f }, {}, Vector3::UnitY);
	mProj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.01f, 10000.0f);
}

void EntityScene::OnBegin()
{
}

void EntityScene::OnEnd()
{
}

void EntityScene::OnPause()
{
}

void EntityScene::OnResume()
{
}

void EntityScene::OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse)
{
	const float dt = (float)timer.GetElapsedSeconds();
	const float tt = (float)timer.GetTotalSeconds();
	const float speed = 100.0f * dt;

	std::vector<Collision::HitPair> collisions;
	mCollision.Collide(collisions);
	if (collisions.empty())
	{
		mVan.transform.DeltaTranslate(mVan.transform.Forward() * speed);
	}
	else
	{
		for (const Collision::HitPair& collision : collisions)
		{
			mVan.transform.DeltaTranslate(collision.mtv);
		}
	}
	mVan.UpdateCollider(mCollision);

	CapsuleCollider& vanCollider = mCollision.mDynamicCapsules.Get(mVan.colliderId);
	SphereCollider& tdCollider = mCollision.mStaticSpheres.Get(mTd.colliderId);
	mColor = vanCollider.IsColliding(tdCollider) ? Colors::Red : Colors::Green;
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	CapsuleCollider& vanCollider = mCollision.mDynamicCapsules.Get(mVan.colliderId);
	SphereCollider& tdCollider = mCollision.mStaticSpheres.Get(mTd.colliderId);
	Debug::Draw(vanCollider.g, mView, mProj, graphics, mColor);
	Debug::Draw(tdCollider.g, mView, mProj, graphics, mColor);

	sPlayerRenderer.Render(mVan.transform.World(), mView, mProj, graphics);
	sBuildingRenderer.Render(mTd.transform.World(), mView, mProj, graphics);
}
