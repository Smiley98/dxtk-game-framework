#pragma once
#include <DirectXMath.h>
#include <array>

class Scene;
namespace Tile
{
	struct Cell
	{
		size_t col;	// x
		size_t row;	// y
	};

	enum Type : size_t
	{
		AIR,
		GRASS,
		WATER,
		ROCK,
		COUNT
	};

	constexpr size_t MAP_SIZE = 10;
	using Map = std::array<std::array<size_t, MAP_SIZE>, MAP_SIZE>;

	void RenderTile(Type type, const Cell& cell, const Scene& scene);
	void Render(const Map& map, const Scene& scene);

	Cell PointToCell(const Vector3& point, const Scene& scene);
	Vector3 CellToPoint(const Cell& cell, const Scene& scene);

	DirectX::XMVECTOR Color(Type type);
	float Cost(Type type);
}
