#pragma once
#include "TileMap.h"
#include "MathUtilities.h"
#include <vector>

class scene;
namespace Pathing {
    using Path = std::vector<Tile::Cell>;
    
    // Returns a vector of tile grid indices to traverse to get from start to end (using A*).
    Path FindPath(const Tile::Cell& start, const Tile::Cell& end, const Tile::Map& map);
    
    // Returns adjacent cells which can be moved to (tiles who's value is EMapValue::AIR).
    std::vector<Tile::Cell> GetNeighbours(const Tile::Cell& cell, const Tile::Map& map);
};