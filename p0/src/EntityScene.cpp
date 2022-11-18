#include "pch.h"
#include "EntityScene.h"
#include "PlayerFactory.h"
#include "BuildingFactory.h"
#include "Utility.h"

#define MAP false
#define SPLINE true
#define KEYBOARD true
#define GAMEPAD false

using namespace DirectX;

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	float step = mWorldWidth / mTestBuildings.size();
	for (size_t i = 0; i < mTestBuildings.size(); i++)
		mTestBuildings[i] = CreateBuilding(mComponents, (Building::Type)i, sBuildingRenderer);

	for (size_t i = 0; i < mTestBuildings.size(); i++)
	{
		Transform3* a = mComponents.transforms.GetComponent(mTestBuildings[i]);
		Transform3* b = mComponents.capsules.GetComponent(mTestBuildings[i])->transform;
		Print(a->Scaling());
		Print(b->Scaling());
	}

	mPlayer = CreatePlayer(mComponents, sPlayerRenderer);

#if SPLINE
	mSpeedTable = CreateSpeedTable(mSpline, 16);
	mHeadlights.SetParent(mComponents.transforms.GetComponent(mPlayer));
	mHeadlights.TranslateY(80.0f);
	mHeadlights.Scale(100.0f);
#endif

#if MAP
	//mPlayer.Load(sPlayerRenderer, mColliders);
	//mPlayer.transform->SetYaw(-45.0f);
	std::vector<Collision2::CapsuleCollider>& capsules = mColliders2.mStaticCapsules.Objects();

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
	//mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 1000.0f }, Vector3::Zero, Vector3::Up);
	mView = Matrix::CreateLookAt(
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 1000.0f },
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f },
		Vector3::UnitY);
#endif
	//mProj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.1f, 10000.0f);
	mProj = Matrix::CreateOrthographic(mWorldWidth, mWorldHeight, 0.1f, 10000.0f);
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
	Transform3& transform = *mComponents.transforms.GetComponent(mPlayer);

#if SPLINE
	Vector3 a = Catmull(DistanceToInterpolation(d, mSpeedTable, interval, sample), interval, mSpline);
	d += lv;
	UpdateCatmull(d, interval, sample, mSpline, mSpeedTable);
	Vector3 b = Catmull(DistanceToInterpolation(d, mSpeedTable, interval, sample), interval, mSpline);
	Vector3 forward = b - a;
	forward.Normalize();
	transform.Translate(a);
	transform.Orientate(forward);
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
		transform.DeltaRotateZ(av);
	if (state.D)
		transform.DeltaRotateZ(-av);

	if (state.W)
		transform.DeltaTranslate(transform.Forward() * av);
	if (state.S)
		transform.DeltaTranslate(transform.Forward() * -av);
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
	//for (Entity i : mTestBuildings)
	//{
	//	Transform3& transform = *mComponents.capsules.GetComponent(i)->transform;
	//	Debug::Draw(*mComponents.capsules.GetComponent(i), mView, mProj, graphics, Colors::Red);
	//}

#if SPLINE
	for (const Vector3& position : mSpline)
		Debug::Primitive(Debug::SPHERE,
			Matrix::CreateScale(50.0f) * Matrix::CreateTranslation(position), mView, mProj, graphics);
	sMiscRenderer.Cone(mHeadlights.World(), mView, mProj, graphics);
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
#endif

	Debug::Draw(*mComponents.capsules.GetComponent(mPlayer), mView, mProj, graphics);
	sPlayerRenderer.Render(mComponents.transforms.GetComponent(mPlayer)->World(), mView, mProj, graphics);
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

// Building + collider render test (no need to rotate -- colliders fit their bounds)!
//std::vector<Building> buildings(Building::COUNT);
//buildings[0].type = Building::TD;
//buildings[1].type = Building::APARTMENT;
//buildings[2].type = Building::BMO;
//buildings[3].type = Building::CONDO;
//buildings[4].type = Building::DUPLEX;
//buildings[5].type = Building::OFFICE;
//buildings[6].type = Building::PENTA;
//buildings[7].type = Building::PINK;
//float step = mWorldWidth / buildings.size();
//for (size_t i = 0; i < buildings.size(); i++)
//{
//	Transform3 tr;
//	Collision2::CapsuleCollider cc;
//	cc.transform = &tr;
//	tr.Translate(100.0f + step * i, mWorldHeight * 0.5f, 0.0f);
//
//	Vector3 bounds = sBuildingRenderer.Bounds(buildings[i].type);
//	float radius = bounds.x;
//	float halfHeight = bounds.y - radius;
//	cc.radius = radius;
//	cc.halfHeight = halfHeight;
//
//	Debug::Draw(cc, mView, mProj, graphics, Colors::Red);
//	sBuildingRenderer.Render(buildings[i], tr.World(), mView, mProj, graphics);
//}
