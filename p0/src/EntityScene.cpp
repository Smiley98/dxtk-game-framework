#include "pch.h"
#include "EntityScene.h"
#include "DebugRenderer.h"
#include "Map.h"
#include "Utility.h"
#define MAP true
#define SPLINE false
#define KEYBOARD true
#define GAMEPAD false

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
#if SPLINE
	mSpeedTable = CreateSpeedTable(mSpline, 16);
	mHeadlights.SetParent(&mVan);
	mHeadlights.TranslateY(80.0f);
	mHeadlights.Scale(100.0f);
#endif

#if MAP
	//mPlayer.Load(sPlayerRenderer, mColliders);
	//mPlayer.transform->SetYaw(-45.0f);
	mPlayer2.Load(sPlayerRenderer, mColliders2);
	mPlayer2.transform.RotateZ(-45.0f);

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
			//BuildingId id = mMap.Add(Building::TD, mColliders);
			//building->position = { x, y, 0.0f };
			//mColliders.Get(building->collider)->Translate(building->position);

			BuildingId id = mMap.Add(Building::TD, mColliders2);
			Building* building = mMap.Get(id);
			building->transform.Translate(x, y, 0.0f);
			x += xStep;

			XMVECTOR t, r, s;
			XMMatrixDecompose(&s, &r, &t, building->transform.World());
			Print(Vector3(t));
		}
		x = xStep * 0.5f;
		y += yStep;
	}

	std::vector<Collision2::CapsuleCollider>& capsules = mColliders2.mStaticCapsules.Objects();
	for (Collision2::CapsuleCollider& i : capsules)
	{
		Print(i.transform->World().Forward());
	}
	Print("Test");
#endif
}

EntityScene::~EntityScene()
{
}

void EntityScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 60.0f * XM_RADIANS;
	fovAngleY = aspectRatio < 1.0f ? fovAngleY * 2.0f : fovAngleY;
#if MAP
	mView = Matrix::CreateLookAt(
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 1000.0f },
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f },
		Vector3::UnitY);
#else
	mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 1000.0f }, Vector3::Zero, Vector3::Up);
#endif
	mProj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.1f, 10000.0f);
	//mProj = Matrix::CreateOrthographic(mWorldWidth, mWorldHeight, 0.1f, 10000.0f);
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
	Vector3 a = Catmull(DistanceToInterpolation(d, mSpeedTable, interval, sample), interval, mSpline);
	d += lv;
	UpdateCatmull(d, interval, sample, mSpline, mSpeedTable);
	Vector3 b = Catmull(DistanceToInterpolation(d, mSpeedTable, interval, sample), interval, mSpline);
	Vector3 forward = b - a;
	forward.Normalize();
	mVan.Translate(a);
	mVan.Orientate(forward);
#endif

#if GAMEPAD
	GamePad::State state = input.gamePad.GetState(0);

	if (state.IsLeftThumbStickLeft())
		mVan.DeltaRotateZ(av);
	if (state.IsLeftThumbStickRight())
		mVan.DeltaRotateZ(-av);

	if (state.IsAPressed())
		mVan.DeltaTranslate(mVan.Forward() * av);
	if (state.IsXPressed())
		mVan.DeltaTranslate(mVan.Forward() * -av);
#endif

#if KEYBOARD
	Keyboard::State state = input.keyboard.GetState();

	if (state.A)
		mPlayer2.transform.DeltaRotateZ(av);
	if (state.D)
		mPlayer2.transform.DeltaRotateZ(-av);

	if (state.W)
		mPlayer2.transform.DeltaTranslate(mPlayer2.transform.Forward() * av);
	if (state.S)
		mPlayer2.transform.DeltaTranslate(mPlayer2.transform.Forward() * -av);
#endif

#if MAP
	//std::vector<Collision::HitPair> collisions;
	//mColliders.Collide(collisions);
	//for (const Collision::HitPair& collision : collisions)
	//{
	//	if (collision.b.tag == Tags::PLAYER)
	//	{
	//		Player& player = *reinterpret_cast<Player*>(collision.b.data);
	//		player.transform->DeltaTranslate(collision.mtv);
	//	}
	//}

	//std::vector<Collision2::HitPair> collisions;
	//mColliders2.Collide(collisions);
	//for (const Collision2::HitPair& collision : collisions)
	//{
	//	if (collision.b.tag == Tags::PLAYER)
	//	{
	//		Player2& player = *reinterpret_cast<Player2*>(collision.b.data);
	//		player.transform.DeltaTranslate(collision.mtv);
	//	}
	//}
#endif
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
#if SPLINE
	for (const Vector3& position : mSpline)
		Debug::Primitive(Debug::SPHERE,
			Matrix::CreateScale(50.0f) * Matrix::CreateTranslation(position), mView, mProj, graphics);
	sMiscRenderer.Cone(mHeadlights.World(), mView, mProj, graphics);
	sPlayerRenderer.Render(mVan.World(), mView, mProj, graphics);
#endif

#if MAP
	//mMap.Render(mView, mProj, graphics);
	std::vector<Collision2::CapsuleCollider>& capsules = mColliders2.mStaticCapsules.Objects();
	//Collision2::CapsuleCollider& capsule = capsules[0];
	//Debug::Draw(capsule, mView, mProj, graphics);
	for (Collision2::CapsuleCollider& i : capsules)
	{
		Debug::Draw(i, mView, mProj, graphics);
	}
	sPlayerRenderer.Render(mPlayer2.transform.World(), mView, mProj, graphics);
	//sPlayerRenderer.Render(mPlayer.transform->World(), mView, mProj, graphics);
#endif
}

// Timer test:
//AddTimer("test", 1.0f, [this]() {
//	Print(mVan.Euler());
//	Print(mVan.Forward());
//}, true);

// Triangle test:
//sMiscRenderer.Triangle(
//	{ 0.0f, 0.0f, 0.0f }, { 100.0f, -100.0f, 0.0f }, { -100.0f, -100.0f, 0.0f },
//mView, mProj, graphics);

// FoV test:
// Note that InRange() is three-dimensional so be aware of the z-component (2d != 3d).
// *top.z = bot.z = bounds.z * 2.0f;
//float length = 100.0f;
//Vector3 bounds = sPlayerRenderer.Bounds(Objects::VAN);
//Vector3 top = mVan.transform->Translation() + forward * bounds.y;
//Vector3 bot = mVan.transform->Translation() - forward * bounds.y;
//Debug::InRange(mHeadlights, mBuilding.position, length * 2.0f, fov, mView, mProj, graphics);

/*
if (state.A)
	mPlayer.transform->DeltaYaw(av);
if (state.D)
	mPlayer.transform->DeltaYaw(-av);

if (state.W)
	mPlayer.transform->DeltaTranslate(mPlayer.transform->Forward() * av);
if (state.S)
	mPlayer.transform->DeltaTranslate(mPlayer.transform->Forward() * -av);
*/