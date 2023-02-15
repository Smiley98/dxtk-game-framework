#pragma once
#include <DirectXMath.h>
#include <array>

namespace Tile
{
    enum Type : int
    {
        AIR,
        GRASS,
        WATER,
        ROCK,
        COUNT
    };

	struct Node;
	struct Cell
	{
		int col = -1;	// x
		int row = -1;	// y
	};

	bool operator==(const Cell& a, const Cell& b);
    bool Compare(const Node& a, const Node& b);

    inline float Manhattan(const Cell& a, const Cell& b) {
        return fabs((float)a.col - (float)b.col) + abs((float)a.row - (float)b.row);
    };

    inline float Euclidean(const Cell& a, const Cell& b) {
        float dx = (float)a.col - (float)b.col;
        float dy = (float)a.row - (float)b.row;
        return sqrtf(dx * dx + dy * dy);
    };

	constexpr size_t MAP_SIZE = 10U;
	using Map = std::array<std::array<int, MAP_SIZE>, MAP_SIZE>;

    std::vector<Cell> Neighbours(const Cell& cell, const Map& map);
    inline int Index(const Cell& cell) { return cell.row * MAP_SIZE + cell.col; };

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
            Init();
        }

        Node(const Cell& cell)
        {
            Init(cell);
        }

        Node(const Cell& cell, float g, float h)
        {
            Init(cell, {}, g, h);
        }

        Node(const Cell& cell, const Cell& parentCell, float g, float h)
        {
            Init(cell, parentCell, g, h);
        }

        void Init(const Cell& cell = {}, const Cell& parent = {}, float g = 0.0f, float h = 0.0f)
        {
            this->cell = cell;
            this->parent = parent;
            this->g = g;
            this->h = h;
        }

        float F() const { return g + h; }

        void Render() const;

        Cell cell, parent;
        float g, h;
    };
}
