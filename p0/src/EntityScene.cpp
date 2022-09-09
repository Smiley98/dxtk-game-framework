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

inline float Random(float min, float max)
{
	return min + (rand() / ((float)RAND_MAX / (max - min)));
}

// This is a 2.5D engine so it makes more sense to do sphere-in-cone or sphere-in-triangle
//inline bool InFOV(const RigidTransform& target, const RigidTransform& viewer, float fov)
//{
//	Vector3 toTarget = target.Translation() - viewer.Translation();
//	float distance = toTarget.Length();
//	toTarget /= distance;
//
//	// true if NOT in FOV:
//	//Math::dot(toSprite, Math::direction(viewer.m_angle)) <= cosf(Math::radians(viewer.m_fov) * 0.5f)
//}

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	const RECT size = graphics->GetOutputSize();
	const float width = float(size.right - size.left);
	const float height = float(size.bottom - size.top);

	mVan.Load(sPlayerRenderer, mColliders);
	mVan.transform->DeltaTranslate(250.0f, 250.0f);
	mVan.transform->DeltaYaw(45.0f);
	mVan.transform->SetForward(mVan.transform->Forward());

	AddTimer("test", 1.0f, [this]() {
		Print(mVan.transform->Forward());
	}, true);

	// Barycentric coordinates essentially map 3d to 2d ie 3d positions to 2d texture coordinates
	//Vector3 x = Vector3::Barycentric(a, b, c, 0.0f, 0.0f);
	//Vector3 y = Vector3::Barycentric(a, b, c, 1.0f, 0.0f);
	//Vector3 z = Vector3::Barycentric(a, b, c, 0.0f, 1.0f);

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

	mCamera.transform.DeltaTranslate(width * 0.5f, height * 0.5f);
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

	mCamera.transform.SetYaw(90.0f + cosf(tt) * 0.4f * DEGREES);

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
	//sPlayerRenderer.Render(mCamera.transform.World(), mView, mProj, graphics);
	//sPlayerRenderer.Render(mVan.transform->World(), mView, mProj, graphics);
	//sMiscRenderer.Triangle({ 250.0f, 250.0f, 100.0f }, { 500.0f, 250.0f, 100.0f }, { 375.0f, 500.0f, 100.0f }, mView, mProj, graphics);
	sMiscRenderer.Triangle({ 250.0f, 250.0f, 100.0f }, { 500.0f, 250.0f, 100.0f }, { 375.0f, 500.0f, 100.0f }, mView, mProj, graphics);

	Vector3 forward = mVan.transform->Forward();
	Vector3 bounds = sPlayerRenderer.Bounds(Objects::VAN);
	Vector3 top = mVan.transform->Translation() + forward * bounds.y;
	Vector3 bot = mVan.transform->Translation() - forward * bounds.y;
	top.z = bot.z = bounds.z * 2.0f;
	//Debug::Line(bot, top, 10.0f, mView, mProj, graphics);

	mHeadlights.SetForward(forward);
	mHeadlights.Translate(top);
	mHeadlights.Scale(100.0f);
	//sMiscRenderer.Cone(mHeadlights.World(), mView, mProj, graphics);

#if MAP
	mMap.Render(mView, mProj, graphics);
#endif
}