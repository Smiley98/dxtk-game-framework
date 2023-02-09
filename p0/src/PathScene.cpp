#include "pch.h"
#include "PathScene.h"

#include "DebugRenderer.h"
#include "Utility.h"

using namespace DirectX;

PathScene::PathScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	: Scene(graphics, audio)
{
}

PathScene::~PathScene()
{
}

void PathScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 60.0f * XM_RADIANS;
	fovAngleY = aspectRatio < 1.0f ? fovAngleY * 2.0f : fovAngleY;
	float halfWidth = mWorldWidth * 0.5f;
	float halfHeight = mWorldHeight * 0.5f;
	mView = Matrix::CreateLookAt({ halfWidth, halfHeight, 1000.0f }, { halfWidth, halfHeight, 0.0f }, Vector3::Up);
	mProj = Matrix::CreateOrthographic(mWorldWidth, mWorldHeight, 0.1f, 10000.0f);
}

void PathScene::OnBegin()
{
	AddTimer("mouse", 0.1f, [&] {
		Mouse::State mouse = Mouse::Get().GetState();
		Tile::Cell cell = Tile::PointToCell({ (float)mouse.x, (float)mouse.y, 0.0f }, *this);
		Print("row: " + std::to_string(cell.row) + " col :" + std::to_string(cell.col));
	}, true);
}

void PathScene::OnEnd()
{
}

void PathScene::OnPause()
{
}

void PathScene::OnResume()
{
}

void PathScene::OnUpdate(float dt, float tt)
{
}

void PathScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	Tile::Render(mMap, *this);
	sPlayerRenderer.Render(sComponents.GetTransform(sPlayer).World(), mView, mProj, graphics);
}