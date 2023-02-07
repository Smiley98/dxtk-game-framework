#pragma once
#include <DirectXMath.h>
#include <array>
#include <vector>

namespace Pathing
{
	struct Cell {
		size_t row;	// y
		size_t col;	// x
	};

	bool operator==(const Cell& a, const Cell& b)
	{
		return a.row == b.row && a.col == b.col;
	}

	enum TileType : size_t
	{
		AIR,
		GRASS,
		WATER,
		ROCK,
		COUNT
	};

	struct Tile : public Cell
	{
		TileType type;
	};

	class TileMap
	{
	public:


	private:
		std::vector<std::vector<Tile>> mTiles;

		static std::array<DirectX::XMVECTOR, TileType::COUNT> sTileColors;
		static std::array<float, TileType::COUNT> sTileCosts;
	};

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
}
