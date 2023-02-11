#include "pch.h"
#include "EntityScene.h"

#include "BuildingFactory.h"
#include "SteeringFactory.h"

#include "PlayerSystem.h"
#include "SteeringSystem.h"
#include "DynamicsSystem.h"
#include "CollisionSystem.h"

#include "Utility.h"

#define MAP false

namespace
{
	constexpr float r = 50.0f;
	constexpr float hh = 150.0f;
}

using namespace DirectX;

EntityScene::EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	: Scene(graphics, audio)
{
#if MAP
	mMap = CreateMap(Map::MINTY_AFTERSHAVE, sComponents, sBuildingRenderer, mSpace.worldWidth, mSpace.worldHeight);
#else
	float step = mSpace.worldWidth / mTestBuildings.size();
	for (size_t i = 0; i < mTestBuildings.size(); i++)
	{
		mTestBuildings[i] = CreateBuilding(sComponents, (Building::Type)i, sBuildingRenderer);
		sComponents.GetTransform(mTestBuildings[i]).Translate(100.0f + i * step, mSpace.worldHeight * 0.5f, 0.0f);
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
	mSpace.view = Matrix::CreateLookAt(
		{ mSpace.worldWidth * 0.5f, mSpace.worldHeight * 0.5f, 1000.0f },
		{ mSpace.worldWidth * 0.5f, mSpace.worldHeight * 0.5f, 0.0f },
		Vector3::Up);
	mSpace.proj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.1f, 10000.0f);
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

void EntityScene::OnUpdate(float dt, float tt)
{
	Players::Update(sComponents, dt);
	Dynamics::Update(sComponents, dt);
	Collision::Update(sComponents);
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	sPlayerRenderer.DebugPlayer(sPlayer, sComponents, mSpace.view, mSpace.proj, graphics, false);
#if MAP
	sBuildingRenderer.DebugMap(mMap, sComponents, mSpace.view, mSpace.proj, graphics, false);
#else
	for (Entity i : mTestBuildings)
		sBuildingRenderer.DebugBuilding(i, sComponents, mSpace.view, mSpace.proj, graphics, true);
#endif
}
