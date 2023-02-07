#include "pch.h"
#include "TileMap.h"

#include "Scene.h"
#include "DebugRenderer.h"

namespace Pathing
{
	bool operator==(const Cell& a, const Cell& b)
	{
		return a.row == b.row && a.col == b.col;
	}

	std::array<DirectX::XMVECTOR, TileType::COUNT> TileMap::sTileColors
	{
		DirectX::Colors::White,		// Air
		DirectX::Colors::Green,		// Grass
		DirectX::Colors::Blue,		// Water
		DirectX::Colors::DarkGray	// Rock
	};

	std::array<float, TileType::COUNT> TileMap::sTileCosts
	{
		0.0f,	// Air
		10.0f,	// Grass
		50.0f,	// Water
		100.0f	// Rock
	};

	Cell TileMap::Index(const Vector3& screenPoint, const Scene& scene) const
	{
		Vector3 worldPoint = scene.ScreenToWorld(screenPoint);
		worldPoint.y = scene.WorldHeight() - worldPoint.y;
		size_t tileWidth = scene.WorldWidth() / MAP_SIZE;
		size_t tileHeight = scene.WorldHeight() / MAP_SIZE;
		return { (size_t)worldPoint.x / tileWidth, (size_t)worldPoint.y / tileHeight };
	}

	void TileMap::RenderTile(TileType type, const Cell& cell, const Scene& scene) const
	{
		float tileWidth = scene.WorldWidth() / (float)MAP_SIZE;
		float tileHeight = scene.WorldHeight() / (float)MAP_SIZE;
		Vector3 tilePosition{ tileWidth * cell.col, scene.WorldHeight() - tileHeight * cell.row, 0.0f };

		float halfWidth = tileWidth * 0.5f;
		float halfHeight = tileHeight * 0.5f;
		tilePosition = Vector3{ tilePosition.x + halfWidth, tilePosition.y - halfHeight, tilePosition.z };
		Vector3 tileExtents{ halfWidth, halfHeight, 1.0f };

		Debug::DrawBox(tilePosition, tileExtents, Vector3::UnitY, sTileColors[type]);
	}

	void TileMap::Render(const Scene& scene) const
	{
		float colSize = scene.WorldWidth()  / (float)MAP_SIZE;
		float rowSize = scene.WorldHeight() / (float)MAP_SIZE;
		for (size_t row = 0; row < MAP_SIZE; row++)
		{
			for (size_t col = 0; col < MAP_SIZE; col++)
			{
				RenderTile((TileType)mTiles[row][col], { col, row }, scene);
			}
		}
	}

}
