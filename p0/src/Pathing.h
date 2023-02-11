#pragma once
#include "TileMap.h"
#include "MathUtilities.h"
#include <vector>

class scene;
namespace Pathing {
    using Path = std::vector<Tile::Cell>;
    
    // Returns a vector of tile grid indices to traverse to get from start to end (using A*).
    Path FindPath(const Tile::Cell& start, const Tile::Cell& end, const Tile::Map& map);
    Path FindPathDebug(const Tile::Cell& start, const Tile::Cell& end, const Tile::Map& map, std::vector<Tile::Node>& nodes);
    
    // Returns adjacent cells which can be moved to (tiles who's value is EMapValue::AIR).
    std::vector<Tile::Cell> GetNeighbours(const Tile::Cell& cell, const Tile::Map& map);

    // Flatten from 2D to 1D
    inline int Index(const Tile::Cell& cell) { return cell.row * Tile::MAP_SIZE + cell.col; };
};
