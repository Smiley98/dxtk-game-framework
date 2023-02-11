#include "pch.h"
#include "PathScene.h"

#include "DebugRenderer.h"
#include "Utility.h"

using namespace DirectX;
using namespace Tile;
using namespace Pathing;

void PathScene::RenderPath(const Path& path, const Map& map)
{
	if (!path.empty())
	{
		for (const Cell& cell : path)
			RenderTileDebug(DirectX::Colors::Red, cell);

		RenderTileDebug(DirectX::Colors::Cyan, path.front());
		RenderTileDebug(DirectX::Colors::Magenta, path.back());
	}
}

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

	float halfWidth = mSpace.worldWidth * 0.5f;
	float halfHeight = mSpace.worldHeight * 0.5f;
	mSpace.proj = Matrix::CreateOrthographic(mSpace.worldWidth, mSpace.worldHeight, 0.1f, 10000.0f);
	mSpace.view = Matrix::CreateLookAt(
		{ halfWidth, halfHeight, 1000.0f },
		{ halfWidth, halfHeight, 0.0f },
		Vector3::Up
	);

	Tile::OnResize(mSpace.worldWidth, mSpace.worldHeight);
}

void PathScene::OnBegin()
{
	AddTimer("mouse", 0.1f, [&] {
		Cell cell = WorldToCell(mMouseWorld);
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
	Mouse::State mouse = Mouse::Get().GetState();
	mMouseWorld = ScreenToWorld({ (float)mouse.x, (float)mouse.y, 0.0f });
	mPath = FindPath({ 1, 8 }, WorldToCell(mMouseWorld), mMap);
}

void PathScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	RenderMap(mMap);
	RenderPath(mPath, mMap);

	//Cell cell = WorldToCell(mMouseWorld);
	//size_t type = GetType(cell, mMap);
	//RenderTile((Tile::Type)(++type % Tile::COUNT), WorldToCell(mMouseWorld));

	sPlayerRenderer.Render(sComponents.GetTransform(sPlayer).World(), mSpace.view, mSpace.proj, graphics);
}
