#include "pch.h"
#include "EntityScene.h"
#include "DebugRenderer.h"
#include "Map.h"

#define VAN true

namespace
{
	constexpr float r = 25.0f;
	constexpr float hh = 75.0f;
}

using namespace DirectX;

float Random(float min, float max)
{
	return min + (rand() / ((float)RAND_MAX / (max - min)));
}

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
#if VAN
	mBuilding = mMap.Add(Buildings::TD, mColliders);
	mVan.Load(sPlayerRenderer, mColliders);
	mVan.transform->Translate({ -500.0f, -500.0f, 0.0f });
	mVan.transform->DeltaRotate(-45.0f);
#else
	const RECT size = graphics->GetOutputSize();
	const float width = float(size.right - size.left);
	const float height = float(size.bottom - size.top);

	for (size_t i = 0; i < mStatic.size(); i++)
	{
		mColliders.Add(mStatic[i], hh, r);
	}

	for (size_t i = 0; i < mDynamic.size(); i++)
	{
		mColliders.Add(mDynamic[i], hh, r);
	}

	auto* s0 = mColliders.Get(mStatic[0]);
	auto* s1 = mColliders.Get(mStatic[1]);
	
	auto* d0 = mColliders.Get(mDynamic[0]);
	auto* d1 = mColliders.Get(mDynamic[1]);

	s0->DeltaTranslate(-width * 0.25f, 0.0f);
	s1->DeltaTranslate( width * 0.25f, 0.0f);

	d0->DeltaTranslate(-width * 0.25f - hh, 0.0f);
	d1->DeltaTranslate( width * 0.25f + hh, 0.0f);
	d0->DeltaRotate(-45.0f);
	d1->DeltaRotate( 45.0f);
#endif
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
#if VAN
	mView = Matrix::CreateLookAt({ 0.0f, -100.0f, 1000.0f }, {}, Vector3::UnitY);
	mProj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.01f, 10000.0f);
#else
	mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 100.0f }, {}, Vector3::UnitY);
	mProj = Matrix::CreateOrthographic(width, height, 0.01f, 1000.0f);
#endif
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
#if VAN
	// Successful auto van-building resolution!
	std::vector<Collision::HitPair> collisions;
	mColliders.Collide(collisions);
	if (collisions.empty())
	{
		mVan.transform->DeltaTranslate(mVan.transform->Forward() * speed);
	}
	else
	{
		for (const Collision::HitPair& collision : collisions)
		{
			if (collision.b.tag == Tags::PLAYER)
			{
				Player& player = *reinterpret_cast<Player*>(collision.b.data);
				player.transform->DeltaTranslate(collision.mtv);
			}
		}
	}
	mColor = mColliders.Get(mVan.id)->IsColliding(*mColliders.Get(mMap.Get(mBuilding)->collider)) ? Colors::Red : Colors::Green;
#else
	// Just additional proof that we can auto-resolve *shallow* collisions.
	// Moving a player around a level will be the ultimate test :)
	std::vector<Collision::HitPair> collisions;
	mColliders.Collide(collisions);
	for (size_t i = 0; i < collisions.size(); i++)
	{
		mColliders.Get(mDynamic[i])->DeltaTranslate(collisions[i].mtv);
		mColliders.Remove(mDynamic[i]);
	}
#endif
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	using namespace Collision;
	using namespace Objects;
#if VAN
	CapsuleCollider* vanCollider = mColliders.Get(mVan.id);
	CapsuleCollider* buildingCollider = mColliders.Get(mMap.Get(mBuilding)->collider);
	Debug::Draw(*vanCollider, mView, mProj, graphics, mColor);
	Debug::Draw(*buildingCollider, mView, mProj, graphics, mColor);
	sPlayerRenderer.Render(mVan.transform->World(), mView, mProj, graphics);
	Buildings::Draw(*mMap.Get(mBuilding), mView, mProj, graphics);
#else
	for (StaticCapsule collider : mStatic)
	{
		Debug::Draw(*mColliders.Get(collider), mView, mProj, graphics);
	}

	for (DynamicCapsule collider : mDynamic)
	{
		CapsuleCollider* capsule = mColliders.Get(collider);
		if (capsule != nullptr)
			Debug::Draw(*capsule, mView, mProj, graphics);
	}
#endif
}
