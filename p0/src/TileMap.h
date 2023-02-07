#pragma once
#include <DirectXMath.h>
#include <array>
#include <vector>

class Scene;
namespace Pathing
{
	struct Cell
	{
		size_t col;	// x
		size_t row;	// y
	};

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

	constexpr size_t MAP_SIZE = 10;

	// Draw in world-space and just convert indices
	class TileMap
	{
	public:
		Cell Index(const Vector3& screenPoint, const Scene& scene) const;
		void RenderTile(TileType type, const Cell& cell, const Scene& scene) const;
		void Render(const Scene& scene) const;

	private:
		// outer = rows, inner = columns
		std::array<std::array<size_t, MAP_SIZE>, MAP_SIZE> mTiles
		{
			std::array<size_t, MAP_SIZE>{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
			std::array<size_t, MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			std::array<size_t, MAP_SIZE>{ 3, 0, 1, 1, 1, 1, 1, 1, 1, 0 },
			std::array<size_t, MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
			std::array<size_t, MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
			std::array<size_t, MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
			std::array<size_t, MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
			std::array<size_t, MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
			std::array<size_t, MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
			std::array<size_t, MAP_SIZE>{ 3, 2, 1, 3, 2, 1, 3, 2, 1, 0 },
		};

		static std::array<DirectX::XMVECTOR, TileType::COUNT> sTileColors;
		static std::array<float, TileType::COUNT> sTileCosts;
	};
}
