#include "pch.h"
#include "EntityScene.h"
#include "DebugRenderer.h"

using namespace DirectX;

float random(float min, float max)
{
	return min + (rand() / ((float)RAND_MAX / (max - min)));
}

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	mTd.Load(sBuildingRenderer, mColliders);
	mVan.Load(sPlayerRenderer, mColliders);
	mVan.transform->Translate({ -500.0f, -500.0f, 0.0f });
	mVan.transform->DeltaRotate(-45.0f);

	/*
	const RECT size = graphics->GetOutputSize();
	const float width = float(size.right - size.left);
	const float height = float(size.bottom - size.top);

	//float edgesRadius = 50.0f;
	//mEdges[0].r = mEdges[1].r = mEdges[2].r = mEdges[3].r = edgesRadius;
	//mEdges[0].hh = mEdges[1].hh = height * 0.5f;
	//mEdges[2].hh = mEdges[3].hh = width * 0.5f;
	//mEdges[0].t.DeltaTranslate(-width * 0.5f, 0.0f);
	//mEdges[1].t.DeltaTranslate(width * 0.5f, 0.0f);
	//mEdges[2].t.DeltaTranslate(0.0f, -height * 0.5f);
	//mEdges[3].t.DeltaTranslate(0.0f, height * 0.5f);
	//mEdges[2].t.DeltaRotate(90.0f);
	//mEdges[3].t.DeltaRotate(90.0f);
	
	// Must make these entities in order to not get memory corruption when calling Collide
	//for (int i = 0; i < 4; i++)
	//{
	//	Collision::Info info;
	//	mCollision.mStaticCapsules.Add(std::move(mEdges[i]), std::move(info));
	//}

	float xMax = width * 0.4f;
	float xMin = xMax * -1.0f;
	float yMax = height * 0.4f;
	float yMin = xMax * -1.0f;
	for (Entity& entity : mSpheres)
	{
		Collision::Info info;
		info.data = &entity;

		Sphere sphere;
		sphere.r = random(15.0f, 25.0f);
		entity.colliderId = mCollision.mDynamicSpheres.Add(std::move(sphere), std::move(info));
		entity.transform.Translate({ random(xMin, xMax), random(yMin, yMax), 0.0f });
	}
	
	for (Entity& entity : mCapsules)
	{
		Collision::Info info;
		info.data = &entity;

		Capsule capsule;
		capsule.r = random(5.0f, 25.0f);
		capsule.hh = random(10.0f, 50.0f);
		entity.colliderId = mCollision.mDynamicCapsules.Add(std::move(capsule), std::move(info));
		entity.transform.Translate({ random(xMin, xMax), random(yMin, yMax), 0.0f });
		entity.transform.DeltaRotate(random(0.0f, 90.0f));
	}
	*/
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
	//mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 100.0f }, {}, Vector3::UnitY);
	//mProj = Matrix::CreateOrthographic(width, height, 0.01f, 1000.0f);
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

	/*
	static float duration = 0.0f;
	static std::vector<Vector3> starts;
	static std::vector<Vector3> ends;
	static std::vector<Entity*> entities;
	static bool run = true;
	if (run)
	{
		run = false;
		std::vector<Collision::HitPair> collisions;
		mCollision.Collide(collisions);
		for (const Collision::HitPair& collision : collisions)
		{
			Entity& a = *reinterpret_cast<Entity*>(collision.infoA.data);
			Entity& b = *reinterpret_cast<Entity*>(collision.infoA.data);
			entities.push_back(reinterpret_cast<Entity*>(collision.infoB.data));
			starts.push_back(b.transform.Translation());
			ends.push_back(b.transform.Translation() + collision.mtv);
		}
	}
	else
	{
		for (size_t i = 0; i < entities.size(); i++)
		{
			Vector3 p = Vector3::Lerp(starts[i], ends[i], std::min(duration, 1.0f));
			entities[i]->transform.Translate(p);
		}
	}
	duration += dt * 0.1f;

	std::vector<Collision::HitPair> collisions;
	mCollision.Collide(collisions);
	for (const Collision::HitPair& collision : collisions)
	{
		Entity& a = *reinterpret_cast<Entity*>(collision.infoA.data);
		Entity& b = *reinterpret_cast<Entity*>(collision.infoA.data);
		a.color = Colors::Red;
		b.color = Colors::Red;
	}*/

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
	mColor = mColliders.Get(mVan.id).IsColliding(mColliders.Get(mTd.id)) ? Colors::Red : Colors::Green;
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	using namespace Collision;
	CapsuleCollider& vanCollider = mColliders.Get(mVan.id);
	SphereCollider& tdCollider = mColliders.Get(mTd.id);
	Debug::Draw(vanCollider, mView, mProj, graphics, mColor);
	Debug::Draw(tdCollider, mView, mProj, graphics, mColor);
	sPlayerRenderer.Render(mVan.transform->World(), mView, mProj, graphics);
	sBuildingRenderer.Render(Matrix::Identity, mView, mProj, graphics);

	/*
	for (Entity& entity : mSpheres)
	{
		SphereCollider& collider = mCollision.mDynamicSpheres.Get(entity.colliderId);
		Debug::Draw(collider.g, mView, mProj, graphics, entity.color, true);
	}
	
	for (Entity& entity : mCapsules)
	{
		CapsuleCollider& collider = mCollision.mDynamicCapsules.Get(entity.colliderId);
		Debug::Draw(collider.g, mView, mProj, graphics, entity.color, true);
	}
	*/
}
