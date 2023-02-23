#pragma once
#include "TileMap.h"
#include "MathUtilities.h"
#include <vector>

using Path = std::vector<Tile::Cell>;	
Path FindPath(const Tile::Cell& start, const Tile::Cell& end, const Tile::Map& map);
Path FindPathDebug(const Tile::Cell& start, const Tile::Cell& end, int steps, const Tile::Map& map, std::vector<Tile::Node>& nodes);
