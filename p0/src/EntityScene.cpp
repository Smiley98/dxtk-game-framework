#include "pch.h"
#include "EntityScene.h"
#include "DebugRenderer.h"
#include "Map.h"
#include "Utility.h"
#define MAP false

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
	const RECT size = graphics->GetOutputSize();
	const float width = float(size.right - size.left);
	const float height = float(size.bottom - size.top);

	mVan.Load(sPlayerRenderer, mColliders);
	mVan.transform->DeltaTranslate(width * 0.25f, height * 0.5f);
	mVan.transform->DeltaYaw(45.0f);
	//mVan.transform->DeltaPitch(45.0f);
	//mVan.transform->Orientate(mVan.transform->Forward());

	AddTimer("test", 1.0f, [this]() {
		Print(mVan.transform->Forward());
	}, true);

#if MAP
	const int rows = 4;
	const int cols = 8;
	const float xStep = mWorldWidth / cols;
	const float yStep = mWorldHeight / rows;
	float x = xStep * 0.5f;
	float y = yStep * 0.5f;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			BuildingId id = mMap.Add(Building::TD, mColliders);
			Building* building = mMap.Get(id);
			building->position = { x, y, 0.0f };
			mColliders.Get(building->collider)->Translate(building->position);
			x += xStep;
		}
		x = xStep * 0.5f;
		y += yStep;
	}
#endif

	//mCamera.position = { width * 0.5f, height * 0.5f };
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
	mView = Matrix::CreateLookAt(
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 1000.0f },
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f },
		Vector3::UnitY);
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

void EntityScene::OnUpdate(float dt, float tt, DX::Input& input)
{
	const float lv = 250.0f * dt;	// linear velocity
	const float av = 100.0f * dt;	// angular velocity

	GamePad::State state = input.gamePad.GetState(0);

	if (state.IsLeftThumbStickLeft())
		mVan.transform->DeltaYaw(av);
	if (state.IsLeftThumbStickRight())
		mVan.transform->DeltaYaw(-av);

	if (state.IsAPressed())
		mVan.transform->DeltaTranslate(mVan.transform->Forward() * av);
	if (state.IsXPressed())
		mVan.transform->DeltaTranslate(mVan.transform->Forward() * -av);

#if MAP
	std::vector<Collision::HitPair> collisions;
	mColliders.Collide(collisions);
	for (const Collision::HitPair& collision : collisions)
	{
		if (collision.b.tag == Tags::PLAYER)
		{
			Player& player = *reinterpret_cast<Player*>(collision.b.data);
			player.transform->DeltaTranslate(collision.mtv);
		}
	}
#endif
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	sPlayerRenderer.Render(mVan.transform->World(), mView, mProj, graphics);
	
	//Transform transform;
	//transform.Scale({ mCamera.range, 10.0f, 1.0f });
	//transform.Translate({ 500.0f, 500.0f, 0.0f });
	//transform.DeltaYaw(mCamera.angle + mCamera.fov * 0.5f);
	//Debug::Primitive(Debug::BOX, transform.World(), mView, mProj, graphics);
	//transform.DeltaYaw(mCamera.angle - mCamera.fov);
	//Debug::Primitive(Debug::BOX, transform.World(), mView, mProj, graphics);

	// cone faces upwards so deal with rotation later and just use boxes for now
	//Debug::Primitive(Debug::CONE,
	//	Matrix::CreateScale(100.0f) *
	//	Matrix::CreateTranslation(mCamera.position),
	//	mView, mProj, graphics);
#if MAP
	mMap.Render(mView, mProj, graphics);
#endif
}