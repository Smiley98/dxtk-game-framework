#include "pch.h"
#include "EntityScene.h"

#include "PlayerFactory.h"
#include "BuildingFactory.h"
#include "SteeringFactory.h"

#include "PlayerSystem.h"
#include "SteeringSystem.h"
#include "DynamicsSystem.h"
#include "CollisionSystem.h"

#include "Utility.h"

#define STEERING false
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
	mRandomTarget = CreateEntity(mComponents);
	mComponents.rigidbodies.Add(mRandomTarget);

	mSeeker = CreateSteering(mComponents, SteeringBehaviour::SEEK, 1000.0f, mPlayer);
	mArriver = CreateSteering(mComponents, SteeringBehaviour::ARRIVE, 1000.0f, mPlayer);
	mRandomSeeker = CreateSteering(mComponents, SteeringBehaviour::SEEK, 1000.0f, mRandomTarget);

	mTarget1 = CreateEntity(mComponents);
	mTarget2 = CreateEntity(mComponents);
	mComponents.spheres.Add(mTarget1).r = 50.0f;
	mComponents.spheres.Add(mTarget2).r = 50.0f;
	
	mAvoider1 = CreateSteering(mComponents, SteeringBehaviour::AVOID, 1000.0f, mTarget1);
	mAvoider2 = CreateSteering(mComponents, SteeringBehaviour::AVOID, 1000.0f, mTarget2);
	Entity collider1 = CreateEntity(mComponents);
	Entity collider2 = CreateEntity(mComponents);
	components.spheres.Add(collider1).r = 50.0f;
	components.spheres.Add(collider2).r = 50.0f;
	components.transforms.GetComponent(collider1)->DeltaTranslateY(100.0f);
	components.transforms.GetComponent(collider2)->DeltaTranslateY(100.0f);
	AddChild(mAvoider1, collider1, mComponents);
	AddChild(mAvoider2, collider2, mComponents);

	AddTimer("RandomTarget", 1.0f, [&] {
		mComponents.transforms.GetComponent(mRandomTarget)->Translate
		(
			Random(0.0f, mWorldWidth),
			Random(0.0f, mWorldHeight),
			0.0f
		);
	}, true);

	AddTimer("targets", 2.0f, [&] {
		static bool flip;
		if (flip)
		{
			mComponents.transforms.GetComponent(mTarget1)->Translate
			(
				mWorldWidth * 0.5f,
				mWorldHeight * 0.25f,
				0.0f
			);

			mComponents.transforms.GetComponent(mTarget2)->Translate
			(
				mWorldWidth * 0.25f,
				mWorldHeight * 0.5f,
				0.0f
			);
		}
		else
		{
			mComponents.transforms.GetComponent(mTarget1)->Translate
			(
				mWorldWidth * 0.5f,
				mWorldHeight * 0.75f,
				0.0f
			);

			mComponents.transforms.GetComponent(mTarget2)->Translate
			(
				mWorldWidth * 0.75f,
				mWorldHeight * 0.5f,
				0.0f
			);
		}
		flip = !flip;
	}, true);
#endif

#if SPLINE
	mSpeedTable = CreateSpeedTable(mSpline, 16);
	mHeadlights = CreateEntity(mComponents);
	mComponents.transforms.GetComponent(mHeadlights)->TranslateY(80.0f);
	mComponents.transforms.GetComponent(mHeadlights)->Scale(100.0f);
	AddChild(mPlayer, mHeadlights, mComponents);
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

	Players::Update(mComponents, input, dt);
	Steering::Update(mComponents, dt);
	Dynamics::Update(mComponents, dt);
	Collision::Update(mComponents);
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	EntityTransform& playerTransform = *mComponents.transforms.GetComponent(mPlayer);
	Capsule& playerCollider = *mComponents.capsules.GetComponent(mPlayer);
	Debug::Capsule(playerTransform, playerCollider.r, playerCollider.hh, mView, mProj, graphics);
	sPlayerRenderer.Render(playerTransform.World(), mView, mProj, graphics);

#if STEERING
	//Debug::Sphere(mComponents.transforms.GetComponent(mSeeker)->Translation(), 50.0f, mView, mProj, graphics);
	//Debug::Sphere(mComponents.transforms.GetComponent(mArriver)->Translation(), 50.0f, mView, mProj, graphics, Colors::PowderBlue);
	//Debug::Sphere(mComponents.transforms.GetComponent(mWanderer)->Translation(), 50.0f, mView, mProj, graphics, Colors::MediumPurple);
	//Debug::Sphere(mComponents.transforms.GetComponent(mRandomSeeker)->Translation(), 50.0f, mView, mProj, graphics, Colors::MediumAquamarine);
	//Debug::Sphere(mComponents.transforms.GetComponent(mRandomTarget)->Translation(), 50.0f, mView, mProj, graphics, Colors::MediumOrchid);
	
	Debug::Sphere(mComponents.transforms.GetComponent(mTarget1)->Translation(), 50.0f, mView, mProj, graphics);
	Debug::Sphere(mComponents.transforms.GetComponent(mTarget2)->Translation(), 50.0f, mView, mProj, graphics);
	Debug::Sphere(mComponents.transforms.GetComponent(mAvoider1)->Translation(), 50.0f, mView, mProj, graphics, Colors::Black);
	Debug::Sphere(mComponents.transforms.GetComponent(mAvoider2)->Translation(), 50.0f, mView, mProj, graphics, Colors::Black);
	Debug::Sphere(mComponents.transforms.GetComponent(
		*mComponents.hierarchies.GetComponent(mAvoider1)->children.begin()
	)->WorldPosition(), 50.0f, mView, mProj, graphics, Colors::Gray);
	Debug::Sphere(mComponents.transforms.GetComponent(
		*mComponents.hierarchies.GetComponent(mAvoider2)->children.begin()
	)->WorldPosition(), 50.0f, mView, mProj, graphics, Colors::Gray);
#endif

#if SPLINE
	for (const Vector3& position : mSpline)
		Debug::Primitive(Debug::SPHERE,
			Matrix::CreateScale(50.0f) * Matrix::CreateTranslation(position), mView, mProj, graphics);

	sMiscRenderer.Cone(mComponents.transforms.GetComponent(mHeadlights)->World(), mView, mProj, graphics);
#endif

#if MAP
	for (Entity i : mBuildings)
	{
		Entity child = *mComponents.hierarchies.GetComponent(i)->children.begin();
		Capsule& collider = *mComponents.capsules.GetComponent(child);
		Building& building = *mComponents.buildings.GetComponent(i);
		EntityTransform& buildingTransform = *mComponents.transforms.GetComponent(i);
		EntityTransform& colliderTransform = *mComponents.transforms.GetComponent(child);

		Debug::Capsule(colliderTransform, collider.r, collider.hh, mView, mProj, graphics);
		sBuildingRenderer.Render(building, buildingTransform.World(), mView, mProj, graphics);
	}
#endif

#if TEST_BUILDINGS
	for (Entity i : mTestBuildings)
	{
		Entity child = *mComponents.hierarchies.GetComponent(i)->children.begin();
		Capsule& collider = *mComponents.capsules.GetComponent(child);
		Building& building = *mComponents.buildings.GetComponent(i);
		EntityTransform& buildingTransform = *mComponents.transforms.GetComponent(i);
		EntityTransform& colliderTransform = *mComponents.transforms.GetComponent(child);

		Debug::Capsule(colliderTransform, collider.r, collider.hh, mView, mProj, graphics);
		sBuildingRenderer.Render(building, buildingTransform.World(), mView, mProj, graphics);
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

// Wandering behaviour test:
//Vector3 wandererPosition = mComponents.transforms.GetComponent(mWanderer)->Translation();
//if (wandererPosition.x > mWorldWidth || wandererPosition.x < 0.0f ||
//	wandererPosition.y > mWorldHeight || wandererPosition.y < 0.0f)
//{
//	mComponents.transforms.GetComponent(mWanderer)->Translate(mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f);
//}

// Position ("teapot") test:
//mParent = CreateEntity(mComponents);
//mChild1 = CreateEntity(mComponents);
//mChild2 = CreateEntity(mComponents);
//EntityTransform& parent = *mComponents.transforms.GetComponent(mParent);
//EntityTransform& child1 = *mComponents.transforms.GetComponent(mChild1);
//EntityTransform& child2 = *mComponents.transforms.GetComponent(mChild2);
//parent.Translate(mWorldWidth * 0.5f, 100.0f, 0.0f);
//parent.DeltaTranslate(100.0f, 100.0f, 100.0f);
//child1.DeltaTranslate(100.0f, 100.0f, 100.0f);
//child2.DeltaTranslate(100.0f, 100.0f, 100.0f);
//parent.DeltaRotateX(45.0f);
//child1.DeltaRotateY(45.0f);
//child2.DeltaRotateZ(45.0f);
//AddChild(mParent, mChild1, mComponents);
//AddChild(mChild1, mChild2, mComponents);

//Debug::Primitive(Debug::TEAPOT,
//	Matrix::CreateScale(50.0f)* mComponents.transforms.GetComponent(mParent)->World(),
//	mView, mProj, graphics, Colors::Red);
//
//Debug::Primitive(Debug::TEAPOT,
//	Matrix::CreateScale(50.0f)* mComponents.transforms.GetComponent(mChild1)->World(),
//	mView, mProj, graphics, Colors::Orange);
//
//Debug::Primitive(Debug::TEAPOT,
//	Matrix::CreateScale(50.0f)* mComponents.transforms.GetComponent(mChild2)->World(),
//	mView, mProj, graphics, Colors::Yellow);
//
//Debug::Primitive(Debug::TEAPOT,
//	Matrix::CreateScale(50.0f)* Matrix::CreateTranslation(mComponents.transforms.GetComponent(mParent)->WorldPosition()),
//	mView, mProj, graphics, Colors::Green);
//
//Debug::Primitive(Debug::TEAPOT,
//	Matrix::CreateScale(50.0f)* Matrix::CreateTranslation(mComponents.transforms.GetComponent(mChild1)->WorldPosition()),
//	mView, mProj, graphics, Colors::Blue);
//
//Debug::Primitive(Debug::TEAPOT,
//	Matrix::CreateScale(50.0f)* Matrix::CreateTranslation(mComponents.transforms.GetComponent(mChild2)->WorldPosition()),
//	mView, mProj, graphics, Colors::Purple);
