#include "pch.h"
#include "EntityScene.h"
#include "PlayerFactory.h"
#include "BuildingFactory.h"
#include "CollisionSystem.h"
#include "DynamicsSystem.h"
#include "PlayerSystem.h"
#include "Utility.h"
#include "SteeringEntity.h"

#define STEERING true
#define SPLINE false
#define MAP false
#define TEST_BUILDINGS true
#define KEYBOARD true
#define GAMEPAD false

using namespace DirectX;

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components)
	: Scene(graphics, audio, components)
{
	mPlayer = CreatePlayer(mComponents, sPlayerRenderer);
	mComponents.transforms.GetComponent(mPlayer)->Translate(800.0f, 450.0f, 0.0f);

#if STEERING
	mSeeker = CreateEntity();
	mComponents.transforms.Add(mSeeker);
	mComponents.rigidbodies.Add(mSeeker);
	mComponents.transforms.GetComponent(mSeeker)->Translate(100.0f, 450.0f, 0.0f);

	mPursuer = CreateEntity();
	mComponents.transforms.Add(mPursuer);
	mComponents.rigidbodies.Add(mPursuer);
	mComponents.transforms.GetComponent(mPursuer)->Translate(100.0f, 450.0f, 0.0f);
#endif

#if SPLINE
	mSpeedTable = CreateSpeedTable(mSpline, 16);
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
			// TODO -- test each building's collider (some half-height's are negative).
			Entity building = CreateBuilding(mComponents, Building::TD, sBuildingRenderer);
			mComponents.transforms.GetComponent(building)->Translate(x, y, 0.0f);
			mBuildings.push_back(building);
			x += xStep;
		}
		x = xStep * 0.5f;
		y += yStep;
	}
#endif

#if TEST_BUILDINGS
	float step = mWorldWidth / mTestBuildings.size();
	for (size_t i = 0; i < mTestBuildings.size(); i++)
	{
		mTestBuildings[i] = CreateBuilding(mComponents, (Building::Type)i, sBuildingRenderer);
		mComponents.transforms.GetComponent(mTestBuildings[i])->Translate(100.0f + i * step, mWorldHeight * 0.5f, 0.0f);
	}
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
#if SPLINE
	mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 1000.0f }, Vector3::Zero, Vector3::Up);
	mProj = Matrix::CreateOrthographic(mWorldWidth, mWorldHeight, 0.1f, 10000.0f);
#else
	mView = Matrix::CreateLookAt(
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 1000.0f },
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f },
		Vector3::Up);
	mProj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.1f, 10000.0f);
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

void EntityScene::OnUpdate(float dt, float tt, const DX::Input& input)
{
	const float lv = 250.0f * dt;	// linear velocity
	const float av = 100.0f * dt;	// angular velocity
	EntityTransform& transform = *mComponents.transforms.GetComponent(mPlayer);

#if STEERING
	// If we wanted a better pursue we could increase/decrease physics prediction steps based on proximity.
	//Steering::Pursue(mPlayer, mPursuer, 1000.0f, dt, mComponents);
	Steering::Arrive(mPlayer, mPursuer, 1000.0f, dt, mComponents);
	Steering::Seek(mPlayer, mSeeker, 1000.0f, mComponents);
	//Steering::Flee(mPlayer, mSeeker, 1000.0f, mComponents);
#endif

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

	Dynamics::Update(mComponents, dt);
	Players::Update(mComponents, input, dt);
	Collision::Update(mComponents);
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	EntityTransform& playerTransform = *mComponents.transforms.GetComponent(mPlayer);
	Capsule& playerCollider = *mComponents.capsules.GetComponent(mPlayer);
	sPlayerRenderer.Render(playerTransform.World(), mView, mProj, graphics);

#if STEERING
	//Debug::Sphere(mComponents.transforms.GetComponent(mSeeker)->Translation(), 50.0f, mView, mProj, graphics);
	//Debug::Sphere(mComponents.transforms.GetComponent(mPursuer)->Translation(), 50.0f, mView, mProj, graphics, Colors::PowderBlue);
#endif

#if SPLINE
	for (const Vector3& position : mSpline)
		Debug::Primitive(Debug::SPHERE,
			Matrix::CreateScale(50.0f) * Matrix::CreateTranslation(position), mView, mProj, graphics);

	// Cannot set the parent up-front since component buffers are reallocated on-add.
	// TODO -- rework parenting system to use entities (ids) so parenting is memory-safe.
	mHeadlights.SetParent(&playerTransform);
	sMiscRenderer.Cone(mHeadlights.World(), mView, mProj, graphics);
#endif

#if MAP
	for (Entity i : mBuildings)
	{
		Transform& transform = *mComponents.transforms.GetComponent(i);
		Capsule& collider = *mComponents.capsules.GetComponent(i);
		Building& building = *mComponents.buildings.GetComponent(i);
		sBuildingRenderer.Render(building, transform.World(), mView, mProj, graphics);
	}
#endif

#if TEST_BUILDINGS
	for (Entity i : mTestBuildings)
	{
		Building& building = *mComponents.buildings.GetComponent(i);
		EntityTransform& transform = *mComponents.transforms.GetComponent(i);
		Capsule& collider = *mComponents.capsules.GetComponent(i);

		// TD, BMO and PENTA (0, 2, 6) are oriented along Z whereas capsules are oriented along Y.
		// Hence, the transforms need to be temporarily rotated for rendering.
		auto temporaryRotate = [&](float x)
		{
			if (building.type == Building::TD ||
				building.type == Building::BMO ||
				building.type == Building::PENTA)
				transform.DeltaRotateX(x);
		};
		
		temporaryRotate(90.0f);
		Debug::Capsule(transform, collider.r, collider.hh, mView, mProj, graphics);
		temporaryRotate(-90.0f);
		sBuildingRenderer.Render(building, transform.World(), mView, mProj, graphics);
	}
	Debug::Capsule(playerTransform, playerCollider.r, playerCollider.hh, mView, mProj, graphics);
#endif
}

// Triangle test:
//sMiscRenderer.Triangle(
//	{ 0.0f, 0.0f, 0.0f }, { 100.0f, -100.0f, 0.0f }, { -100.0f, -100.0f, 0.0f },
//mView, mProj, graphics);

// Timer test:
//AddTimer("test", 1.0f, [this] {
//	Print("Callback");
//}, false);
//
// Process test:
//AddProcess("test", 1.0f / 10.0f, 1.0f, [this](const Process& process) {
//	if (process.Percentage() > 0.5f)
//		RemoveProcess("test");
//	Print(std::to_string(process.Percentage()));
//}, true);
