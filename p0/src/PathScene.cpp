#include "pch.h"
#include "PathScene.h"

#include "DebugRenderer.h"
#include "Utility.h"

using namespace DirectX;
using namespace Tile;
using namespace Pathing;

void PathScene::RenderPath(const Path& path)
{
	if (!path.empty())
	{
		for (const Cell& cell : path)
			RenderTileDebug(DirectX::Colors::Red, cell);

		// Start isn't included in Path but End is.
		RenderTileDebug(DirectX::Colors::Cyan, mStart);
		RenderTileDebug(DirectX::Colors::Magenta, mEnd);
	}
}

void PathScene::RenderNode(const Tile::Node& node)
{
	static std::array<DirectX::XMVECTOR, 10> ramp
	{
		DirectX::Colors::Red,
		DirectX::Colors::Orange,
		DirectX::Colors::Yellow,
		DirectX::Colors::Green,
		DirectX::Colors::DarkGreen,
		DirectX::Colors::Blue,
		DirectX::Colors::Magenta,
		DirectX::Colors::DarkMagenta,
		DirectX::Colors::BlueViolet,
		DirectX::Colors::Black
	};

	int score = node.f() < 10 ? node.f() : 9;
	RenderTileDebug(ramp[score], node.cell);
}

PathScene::PathScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	: Scene(graphics, audio)
{
	mStart = { 1, 8 };
	mEnd   = { 8, 1 };

	mOnGui = [&]
	{
		ImGui::SliderInt2("Start", (int*)&mStart, 0, 9);
		ImGui::SliderInt2("End", (int*)&mEnd, 0, 9);
		sMiscRenderer.Text({ 0.0f, 20.0f, 1.0f }, L"Lit!", Colors::Red);
	};
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
	mPath = FindPathDebug(mStart, mEnd, mMap, mNodes);
}

void PathScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	RenderMap(mMap);
	RenderPath(mPath);

	//for (const Node& node : mNodes)
	//	RenderNode(node);

	for (const Cell& cell : mPath)
		RenderNode(mNodes[Index(cell)]);

	//sPlayerRenderer.Render(sComponents.GetTransform(sPlayer).World(), mSpace.view, mSpace.proj, graphics);
}

// Mouse cell test
//AddTimer("mouse", 0.1f, [&]
//{
//	Cell cell = WorldToCell(mMouseWorld);
//	Print("row: " + std::to_string(cell.row) + " col :" + std::to_string(cell.col));
//}, true);
//
//Cell cell = WorldToCell(mMouseWorld);
//size_t type = GetType(cell, mMap);
//RenderTile((Tile::Type)(++type% Tile::COUNT), WorldToCell(mMouseWorld));
