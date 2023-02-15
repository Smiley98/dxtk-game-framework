#include "pch.h"
#include "TileMap.h"

#include "Scene.h"
#include "DebugRenderer.h"

// Tiles are in world-space
namespace Tile
{
	float gTileWidth = 0;
	float gTileHeight = 0;

	bool operator==(const Cell& a, const Cell& b)
	{
		return a.row == b.row && a.col == b.col;
	}

	// priority_queue orders its elements *GREATEST* to least, but we want its elements
	// least-to-greatest in order to obtain the best rather than the worst path!
	bool Compare(const Node& a, const Node& b)
	{
		return a.F() > b.F();
	}

	std::vector<Cell> Neighbours(const Cell& cell, const Map& map)
	{
		std::vector<Cell> cells;
		// Diagonals
		for (int row = cell.row - 1; row <= cell.row + 1 && row >= 0 && row < MAP_SIZE; row++)
		{
			for (int col = cell.col - 1; col <= cell.col + 1 && col >= 0 && col < MAP_SIZE; col++)
			{
				if (!(col == cell.col && row == cell.row))
					cells.push_back({ col, row });
			}
		}
		//if (cell.col - 1 >= 0) cells.push_back({ cell.col - 1, cell.row });
		//if (cell.col + 1 < MAP_SIZE) cells.push_back({ cell.col + 1, cell.row });
		//if (cell.row - 1 >= 0) cells.push_back({ cell.col, cell.row - 1 });
		//if (cell.row + 1 < MAP_SIZE) cells.push_back({ cell.col, cell.row + 1 });
		return cells;
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
		return { int(position.x / gTileWidth), int((Scene::WorldHeight() - position.y) / gTileHeight) };
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
		gTileWidth = worldWidth / (float)MAP_SIZE;
		gTileHeight = worldHeight / (float)MAP_SIZE;
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

		int index = (int)F() <= MAP_SIZE - 1 ? (int)F() : 9;
		RenderTileDebug(ramp[index], cell);

		Vector3 position(CellToWorld(cell));
		Debug::DrawText(position, std::to_wstring(F()));
	}
}
