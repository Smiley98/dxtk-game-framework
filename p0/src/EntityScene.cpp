#include "pch.h"
#include "EntityScene.h"
#include "DebugRenderer.h"
#include "Map.h"
#include "Utility.h"
#include "Curves.h"
#define MAP false
#define TIMER false
#define SPLINE true

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

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	const RECT size = graphics->GetOutputSize();
	const float width = float(size.right - size.left);
	const float height = float(size.bottom - size.top);

	mVan.Load(sPlayerRenderer, mColliders);
	mVan.transform->SetYaw(-45.0f);

	mBuilding.type = Building::Type::TD;
	mBuilding.position = { -500.0f, 300.0f, 0.0f };

#if TIMER
	AddTimer("test", 1.0f, [this]() {
		Print(mVan.transform->Forward());
	}, true);
#endif

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
	//mView = Matrix::CreateLookAt(
	//	{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 1000.0f },
	//	{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f },
	//	Vector3::UnitY);
	mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 1000.0f }, Vector3::Zero, Vector3::Up);
	//mProj = Matrix::CreateOrthographic(mWorldWidth, mWorldHeight, 0.01f, 10000.0f);
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

#if SPLINE
	Vector3 p0, p1, p2, p3;
	IndexCatmull(mSpline, i, p0, p1, p2, p3);
	mVan.transform->Translate(Vector3::CatmullRom(p0, p1, p2, p3, t));
	if (t >= 1.0f)
	{
		t = 0.0f;
		i++;
	}
	t += dt;
#else
	GamePad::State state = input.gamePad.GetState(0);

	if (state.IsLeftThumbStickLeft())
		mVan.transform->DeltaYaw(av);
	if (state.IsLeftThumbStickRight())
		mVan.transform->DeltaYaw(-av);

	if (state.IsAPressed())
		mVan.transform->DeltaTranslate(mVan.transform->Forward() * av);
	if (state.IsXPressed())
		mVan.transform->DeltaTranslate(mVan.transform->Forward() * -av);
#endif

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
	//sMiscRenderer.Triangle({ 0.0f, 0.0f, 0.0f }, { 100.0f, -100.0f, 0.0f }, { -100.0f, -100.0f, 0.0f }, mView, mProj, graphics);
	sPlayerRenderer.Render(mVan.transform->World(), mView, mProj, graphics);
	Building::Draw(mBuilding, mView, mProj, graphics);

	Vector3 forward = mVan.transform->Forward();
	Vector3 bounds = sPlayerRenderer.Bounds(Objects::VAN);
	Vector3 top = mVan.transform->Translation() + forward * bounds.y;
	Vector3 bot = mVan.transform->Translation() - forward * bounds.y;

	float length = 100.0f;
	mHeadlights.SetForward(forward);
	mHeadlights.Translate(top);
	mHeadlights.Scale(length);
	sMiscRenderer.Cone(mHeadlights.World(), mView, mProj, graphics);
	Debug::InRange(mHeadlights, mBuilding.position, length * 2.0f, fov, mView, mProj, graphics);

	for (const Vector3& position : mSpline)
	{
		Debug::Primitive(Debug::SPHERE, Matrix::CreateScale(50.0f) * Matrix::CreateTranslation(position), mView, mProj, graphics);
	}


	//*top.z = bot.z = bounds.z * 2.0f;*
	// This was done so that we could render a line about the van to prevent it from being occluded
	// Doing so screws up the FOV check so I've removed it
	// TODO -- Transform2D and Transform3D so these issues don't happen

#if MAP
	mMap.Render(mView, mProj, graphics);
#endif
}
