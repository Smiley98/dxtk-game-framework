#pragma once
#include <DirectXMath.h>
#include <array>

namespace Tile
{
	struct Node;
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

	void RenderTileDebug(DirectX::XMVECTOR color, const Cell& cell);
	void RenderTile(Type type, const Cell& cell);
	void RenderMap(const Map& map);

	Cell WorldToCell(const Vector3& position);
	Vector3 CellToWorld(const Cell& cell);
	void OnResize(float worldWidth, float worldHeight);

	DirectX::XMVECTOR Color(Type type);
	float Cost(Type type);
	Type GetType(const Cell& cell, const Map& map);

    struct Node
    {
        Node()
        {
            init();
        }

        Node(const Cell& cell)
        {
            init(cell);
        }

        Node(const Cell& cell, int g, int h)
        {
            init(cell, {}, g, h);
        }

        Node(const Cell& cell, const Cell& parentCell, int g, int h)
        {
            init(cell, parentCell, g, h);
        }

        void init(const Cell& cell = {}, const Cell& parent = {}, int g = 0, int h = 0)
        {
            this->cell = cell;
            this->parent = parent;
            this->g = g;
            this->h = h;
        }

        int f() const { return g + h; }

        void Render() const;

        Cell cell, parent;
        int g, h;
    };
}
