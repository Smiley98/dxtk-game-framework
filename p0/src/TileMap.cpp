#include "pch.h"
#include "TileMap.h"

#include "Scene.h"
#include "DebugRenderer.h"

// Tiles are in world-space
namespace Tile
{
	bool operator==(const Cell& a, const Cell& b)
	{
		return a.row == b.row && a.col == b.col;
	}

	void RenderTile(Type type, const Cell& cell, const Scene& scene)
	{
		float tileWidth = scene.WorldWidth() / (float)MAP_SIZE;
		float tileHeight = scene.WorldHeight() / (float)MAP_SIZE;
		Debug::DrawBox(
			CellToWorld(cell, scene),
			{ tileWidth * 0.5f, tileHeight * 0.5f, 1.0f },
			Vector3::UnitY,
			Color(type)
		);
	}

	void RenderMap(const Map& map, const Scene& scene)
	{
		for (size_t row = 0; row < MAP_SIZE; row++)
		{
			for (size_t col = 0; col < MAP_SIZE; col++)
			{
				RenderTile((Type)map[row][col], { col, row }, scene);
			}
		}
	}

	Cell WorldToCell(const Vector3& position, const Scene& scene)
	{
		size_t tileWidth = scene.WorldWidth() / MAP_SIZE;
		size_t tileHeight = scene.WorldHeight() / MAP_SIZE;
		return { (size_t)position.x / tileWidth, (size_t)(scene.WorldHeight() - position.y) / tileHeight };
	}

	Vector3 CellToWorld(const Cell& cell, const Scene& scene)
	{
		size_t tileWidth = scene.WorldWidth() / MAP_SIZE;
		size_t tileHeight = scene.WorldHeight() / MAP_SIZE;
		return { 
			cell.col * tileWidth + tileWidth * 0.5f,
			scene.WorldHeight() - (cell.row * tileHeight + tileHeight * 0.5f),
			0.0f
		};
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

	Type GetType(const Cell& cell, const Map& map)
	{
		return (Type)map[cell.row][cell.col];
	}
}
