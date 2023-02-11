#include "pch.h"
#include "PathScene.h"

#include "DebugRenderer.h"
#include "Utility.h"

using namespace DirectX;
using namespace Tile;
using namespace Pathing;

void Lerp(const Cell& a, const Cell& b, float t, Vector3& position, Vector3& direction)
{
	Vector3 pA = CellToWorld(a);
	Vector3 pB = CellToWorld(b);
	(pB - pA).Normalize(direction);
	position = Vector3::Lerp(pA, pB, t);
}

PathScene::PathScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	: Scene(graphics, audio)
{
	mStart = { 1, 8 };
	mEnd   = { 8, 1 };

	mOnGui = [&]
	{
		ImGui::SliderInt("Render State", &mPathRenderState, 1, 3);
		if (ImGui::SliderInt2("Start", (int*)&mStart, 0, 9) || ImGui::SliderInt2("End", (int*)&mEnd, 0, 9))
		{
			mPath = FindPathDebug(mStart, mEnd, mMap, mNodes);
			if (!mPath.empty())
			{
				mCurrent = 0;
				mNext = 1;
			}
		}
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
	sComponents.GetTransform(sPlayer).Translate(CellToWorld(mStart));
	mPath = FindPathDebug(mStart, mEnd, mMap, mNodes);
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

	static float time = 0.0f;
	static int index = 0;
	Vector3 position, direction;
	Lerp(mPath[mCurrent], mPath[mNext], time, position, direction);
	EntityTransform& transform = sComponents.GetTransform(sPlayer);
	transform.Translate(position);
	transform.Orientate(direction);
	if (time >= 1.0f)
	{
		time = 0.0f;
		(++mCurrent) %= mPath.size();
		(++mNext) %= mPath.size();
	}
	time += dt;
}

void PathScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	RenderMap(mMap);
	switch (mPathRenderState)
	{
		case 1:
			for (const Cell& cell : mPath)
				RenderTileDebug(DirectX::Colors::LightGreen, cell);
			break;
		case 2:
			for (const Cell& cell : mPath)
				mNodes[Index(cell)].Render();
			break;
		case 3:
			for (const Node& node : mNodes)
				node.Render();
			break;
	}
	
	// Start isn't included in Path but End is.
	RenderTileDebug(DirectX::Colors::Cyan, mStart);
	RenderTileDebug(DirectX::Colors::Red, mEnd);

	sPlayerRenderer.DebugPlayer(sPlayer, sComponents, mSpace.view, mSpace.proj, graphics, false);
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
