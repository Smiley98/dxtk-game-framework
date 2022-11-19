#include "pch.h"
#include "EntityScene.h"
#include "PlayerFactory.h"
#include "BuildingFactory.h"
#include "Utility.h"

#define MAP true
#define SPLINE false
#define KEYBOARD true
#define GAMEPAD false

using namespace DirectX;

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
#if SPLINE
	mSpeedTable = CreateSpeedTable(mSpline, 16);
	mHeadlights.SetParent(mComponents.transforms.GetComponent(mPlayer));
	mHeadlights.TranslateY(80.0f);
	mHeadlights.Scale(100.0f);
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
			Entity building = CreateBuilding(mComponents, Building::TD, sBuildingRenderer);
			mComponents.transforms.GetComponent(building)->Translate(x, y, 0.0f);
			mBuildings.push_back(building);
			x += xStep;
		}
		x = xStep * 0.5f;
		y += yStep;
	}

	//float step = mWorldWidth / mTestBuildings.size();
	//for (size_t i = 0; i < mTestBuildings.size(); i++)
	//{
	//	mTestBuildings[i] = CreateBuilding(mComponents, (Building::Type)i, sBuildingRenderer);
	//	mComponents.transforms.GetComponent(mTestBuildings[i])->Translate(100.0f + i * step, mWorldHeight * 0.5f, 0.0f);
	//}
#endif

	mPlayer = CreatePlayer(mComponents, sPlayerRenderer);
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
		Vector3::Up);
#else
	//mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 1000.0f }, Vector3::Zero, Vector3::Up);
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
		transform.DeltaRotateZ(av);
	if (state.IsLeftThumbStickRight())
		transform.DeltaRotateZ(-av);

	if (state.IsAPressed())
		transform.DeltaTranslate(transform.Forward() * av);
	if (state.IsXPressed())
		transform.DeltaTranslate(transform.Forward() * -av);
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
	std::vector<Collision2::HitPair> collisions;
	Collision2::Collide(mComponents.transforms.Entities(), mComponents, collisions);
	for (const Collision2::HitPair& collision : collisions)
	{
		for (Entity entity : collision.hits)
		{
			Tags::Tag* tag = mComponents.tags.GetComponent(entity);
			if (tag != nullptr)
			{
				switch (*tag)
				{
					case Tags::PLAYER:
						mComponents.transforms.GetComponent(entity)->DeltaTranslate(collision.mtv);
						break;
					case Tags::BUILDING:
						mComponents.buildings.GetComponent(entity)->durability -= 10.0f;
						break;
					case Tags::BULLET:
						break;
				}
			}
		}
	}
#endif
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
#if SPLINE
	for (const Vector3& position : mSpline)
		Debug::Primitive(Debug::SPHERE,
			Matrix::CreateScale(50.0f) * Matrix::CreateTranslation(position), mView, mProj, graphics);
	sMiscRenderer.Cone(mHeadlights.World(), mView, mProj, graphics);
#endif

#if MAP
	//for (Entity i : mTestBuildings)
	for (Entity i : mBuildings)
	{
		Transform3& transform = *mComponents.transforms.GetComponent(i);
		Capsule& collider = *ToCapsule(mComponents.colliders.GetComponent(i));
		Building& building = *mComponents.buildings.GetComponent(i);
		Debug::Draw(transform, collider, mView, mProj, graphics, Colors::Red);
		//sBuildingRenderer.Render(building, transform.World(), mView, mProj, graphics);
	}
#endif

	Transform3& playerTransform = *mComponents.transforms.GetComponent(mPlayer);
	Capsule& playerCollider = *ToCapsule(mComponents.colliders.GetComponent(mPlayer));
	Debug::Draw(playerTransform, playerCollider, mView, mProj, graphics);
	sPlayerRenderer.Render(playerTransform.World(), mView, mProj, graphics);
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
