#include "pch.h"
#include "TileMap.h"

#include "Scene.h"
#include "DebugRenderer.h"

namespace Tile
{
	bool operator==(const Cell& a, const Cell& b)
	{
		return a.row == b.row && a.col == b.col;
	}

	/*
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
	*/

	//Cell TileMap::Index(const Vector3& screenPoint, const Scene& scene) const
	//{
	//	Vector3 worldPoint = scene.ScreenToWorld(screenPoint);
	//	worldPoint.y = scene.WorldHeight() - worldPoint.y;
	//	size_t tileWidth = scene.WorldWidth() / MAP_SIZE;
	//	size_t tileHeight = scene.WorldHeight() / MAP_SIZE;
	//	return { (size_t)worldPoint.x / tileWidth, (size_t)worldPoint.y / tileHeight };
	//}

	void RenderTile(Type type, const Cell& cell, const Scene& scene)
	{
		float tileWidth = scene.WorldWidth() / (float)MAP_SIZE;
		float tileHeight = scene.WorldHeight() / (float)MAP_SIZE;
		Vector3 tilePosition{ tileWidth * cell.col, scene.WorldHeight() - tileHeight * cell.row, 0.0f };

		float halfWidth = tileWidth * 0.5f;
		float halfHeight = tileHeight * 0.5f;
		tilePosition = Vector3{ tilePosition.x + halfWidth, tilePosition.y - halfHeight, tilePosition.z };
		Vector3 tileExtents{ halfWidth, halfHeight, 1.0f };

		Debug::DrawBox(tilePosition, tileExtents, Vector3::UnitY, Color(type));
	}

	void Render(const Map& map, const Scene& scene)
	{
		float colSize = scene.WorldWidth()  / (float)MAP_SIZE;
		float rowSize = scene.WorldHeight() / (float)MAP_SIZE;
		for (size_t row = 0; row < MAP_SIZE; row++)
		{
			for (size_t col = 0; col < MAP_SIZE; col++)
			{
				RenderTile((Type)map[row][col], { col, row }, scene);
			}
		}
	}

	Cell PointToCell(const Vector3& point, const Scene& scene)
	{
		Vector3 worldPoint = scene.ScreenToWorld(point);
		worldPoint.y = scene.WorldHeight() - worldPoint.y;
		size_t tileWidth = scene.WorldWidth() / MAP_SIZE;
		size_t tileHeight = scene.WorldHeight() / MAP_SIZE;
		return { (size_t)worldPoint.x / tileWidth, (size_t)worldPoint.y / tileHeight };
	}

	Vector3 CellToPoint(const Cell& cell, const Scene& scene)
	{

		return Vector3();
	}

	DirectX::XMVECTOR Color(Type type)
	{
		static std::array<DirectX::XMVECTOR, Type::COUNT> colors
		{
			DirectX::Colors::White,		// Air
			DirectX::Colors::Green,		// Grass
			DirectX::Colors::Blue,		// Water
			DirectX::Colors::DarkGray	// Rock
		};
		return colors[type];
	}

	float Cost(Type type)
	{
		static std::array<float, Type::COUNT> costs
		{
			0.0f,	// Air
			10.0f,	// Grass
			50.0f,	// Water
			100.0f	// Rock
		};
		return costs[type];
	}
}
