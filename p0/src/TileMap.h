#pragma once
#include <DirectXMath.h>
#include <array>

class Scene;
namespace Tile
{
	struct Cell
	{
		int col = -1;	// x
		int row = -1;	// y
	};
	bool operator==(const Cell& a, const Cell& b);

	enum Type : int
	{
		AIR,
		GRASS,
		WATER,
		ROCK,
		COUNT
	};

	constexpr size_t MAP_SIZE = 10U;
	using Map = std::array<std::array<int, MAP_SIZE>, MAP_SIZE>;

	void RenderTileDebug(DirectX::XMVECTOR color, const Cell& cell, const Scene& scene);
	void RenderTile(Type type, const Cell& cell, const Scene& scene);
	void RenderMap(const Map& map, const Scene& scene);

	Cell WorldToCell(const Vector3& position, const Scene& scene);
	Vector3 CellToWorld(const Cell& cell, const Scene& scene);

	DirectX::XMVECTOR Color(Type type);
	float Cost(Type type);
	Type GetType(const Cell& cell, const Map& map);
}
