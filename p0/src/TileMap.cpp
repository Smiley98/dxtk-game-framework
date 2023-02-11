#include "pch.h"
#include "TileMap.h"

#include "Scene.h"
#include "DebugRenderer.h"

// Tiles are in world-space
namespace Tile
{
	int gTileWidth = 0;
	int gTileHeight = 0;

	bool operator==(const Cell& a, const Cell& b)
	{
		return a.row == b.row && a.col == b.col;
	}

	void RenderTileDebug(DirectX::XMVECTOR color, const Cell& cell)
	{
		Debug::DrawBox(
			CellToWorld(cell),
			{ gTileWidth * 0.5f, gTileHeight * 0.5f, 1.0f },
			Vector3::UnitY,
			color
		);
	}

	void RenderTile(Type type, const Cell& cell)
	{
		Debug::DrawBox(
			CellToWorld(cell),
			{ gTileWidth * 0.5f, gTileHeight * 0.5f, 1.0f },
			Vector3::UnitY,
			Color(type)
		);
	}

	void RenderMap(const Map& map)
	{
		for (int row = 0; row < MAP_SIZE; row++)
		{
			for (int col = 0; col < MAP_SIZE; col++)
			{
				RenderTile((Type)map[row][col], { col, row });
			}
		}
	}

	Cell WorldToCell(const Vector3& position)
	{
		return { (int)position.x / gTileWidth, (int)(Scene::WorldHeight() - position.y) / gTileHeight };
	}

	Vector3 CellToWorld(const Cell& cell)
	{
		return { 
			cell.col * gTileWidth + gTileWidth * 0.5f,
			Scene::WorldHeight() - (cell.row * gTileHeight + gTileHeight * 0.5f),
			0.0f
		};
	}

	void OnResize(float worldWidth, float worldHeight)
	{
		gTileWidth = worldWidth / MAP_SIZE;
		gTileHeight = worldHeight / MAP_SIZE;
;	}

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

	void Node::Render() const
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

		int index = f() < 10 ? f() : 9;
		RenderTileDebug(ramp[index], cell);

		Vector3 position(CellToWorld(cell));
		Debug::DrawText(position, std::to_wstring(f()));
	}
}
