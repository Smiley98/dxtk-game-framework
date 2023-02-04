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
	mMap = CreateMap(Map::MINTY_AFTERSHAVE, sComponents, sBuildingRenderer, mWorldWidth, mWorldHeight);
#else
	float step = mWorldWidth / mTestBuildings.size();
	for (size_t i = 0; i < mTestBuildings.size(); i++)
	{
		mTestBuildings[i] = CreateBuilding(sComponents, (Building::Type)i, sBuildingRenderer);
		sComponents.transforms.GetComponent(mTestBuildings[i])->Translate(100.0f + i * step, mWorldHeight * 0.5f, 0.0f);
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
	Players::Update(sComponents, input, dt);
	Dynamics::Update(sComponents, dt);
	Collision::Update(sComponents);
}

void EntityScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	sPlayerRenderer.DebugPlayer(sPlayer, sComponents, mView, mProj, graphics, false);
#if MAP
	sBuildingRenderer.DebugMap(mMap, sComponents, mView, mProj, graphics, false);
#else
	for (Entity i : mTestBuildings)
		sBuildingRenderer.DebugBuilding(i, sComponents, mView, mProj, graphics, true);
#endif
}
