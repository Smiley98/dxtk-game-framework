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

#define MAP true

namespace
{
	constexpr float r = 50.0f;
	constexpr float hh = 150.0f;
}

using namespace DirectX;

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components)
	: Scene(graphics, audio, components)
{
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
#else
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
	mView = Matrix::CreateLookAt(
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 1000.0f },
		{ mWorldWidth * 0.5f, mWorldHeight * 0.5f, 0.0f },
		Vector3::Up);
	mProj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.1f, 10000.0f);
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
	Players::Update(mComponents, input, dt);
	Dynamics::Update(mComponents, dt);
	Collision::Update(mComponents);
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	EntityTransform& playerTransform = *mComponents.transforms.GetComponent(sPlayer);
	Collider& playerCollider = *mComponents.colliders.GetComponent(sPlayer);
	Debug::Capsule(playerTransform.WorldPosition(), playerTransform.WorldForward(),
		playerCollider.r, playerCollider.hh, mView, mProj, graphics);
	sPlayerRenderer.Render(playerTransform.World(), mView, mProj, graphics);

#if MAP
	for (Entity i : mBuildings)
	{
		Entity child = *mComponents.hierarchies.GetComponent(i)->children.begin();
		Collider& collider = *mComponents.colliders.GetComponent(child);
		Building& building = *mComponents.buildings.GetComponent(i);
		EntityTransform& buildingTransform = *mComponents.transforms.GetComponent(i);
		EntityTransform& colliderTransform = *mComponents.transforms.GetComponent(child);

		Debug::Capsule(colliderTransform.WorldPosition(), colliderTransform.WorldForward(),
			collider.r, collider.hh, mView, mProj, graphics);
		sBuildingRenderer.Render(building, buildingTransform.World(), mView, mProj, graphics);
	}
#else
	for (Entity i : mTestBuildings)
	{
		Entity child = *mComponents.hierarchies.GetComponent(i)->children.begin();
		Collider& collider = *mComponents.colliders.GetComponent(child);
		Building& building = *mComponents.buildings.GetComponent(i);
		EntityTransform& buildingTransform = *mComponents.transforms.GetComponent(i);
		EntityTransform& colliderTransform = *mComponents.transforms.GetComponent(child);

		Debug::Capsule(colliderTransform.WorldPosition(), colliderTransform.WorldForward(),
			collider.r, collider.hh, mView, mProj, graphics);
		sBuildingRenderer.Render(building, buildingTransform.World(), mView, mProj, graphics);
	}
	Debug::Capsule(playerTransform.WorldPosition(), playerTransform.WorldForward(),
		playerCollider.r, playerCollider.hh, mView, mProj, graphics);
#endif
}
