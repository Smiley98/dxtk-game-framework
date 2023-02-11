#include "pch.h"
#include "Pathing.h"

namespace Pathing {
    using namespace Tile;

    Path FindPath(const Cell& start, const Cell& end, const Map& map)
    {
        std::vector<Node> nodes;
        return FindPathDebug(start, end, map, nodes);
    }

    Path FindPathDebug(const Cell& start, const Cell& end, const Map& map, std::vector<Node>& nodes)
    {
        // Computationally cheap but treats diagonals the same as adjacents
        auto manhattan = [](const Cell& a, const Cell& b) -> int {
            return abs(a.col - b.col) + abs(a.row - b.row);
        };

        // Computationally expensive but treats diagonals as more expensive than adjacents
        auto euclidean = [](const Cell& a, const Cell& b) -> int {
            int dx = a.col - b.col;
            int dy = a.row - b.row;
            return sqrt(dx * dx + dy * dy);
        };

        // priority_queue orders its elements *GREATEST* to least, but we want its elements
        // least-to-greatest in order to obtain the best rather than the worst path!
        auto predicate = [](const Node& a, const Node& b) -> bool { return a.f() > b.f(); };

        // Flatten from 2D to 1D
        auto index = [&](const Cell& cell) -> int { return cell.row * MAP_SIZE + cell.col; };

        // Mark all nodes as unvisited (closed list = false) and append start to open list
        const int tileCount = MAP_SIZE * MAP_SIZE;
        std::vector<Node> tileNodes(tileCount);
        std::priority_queue<Node, std::vector<Node>, decltype(predicate) > openList(predicate);
        std::vector<bool> closedList(tileCount, false);
        tileNodes[index(start)].parent = start;
        openList.push({ start });

        while (!openList.empty())
        {
            const Cell currentCell = openList.top().cell;

            // End condition (destination reached)
            if (currentCell == end)
            {
                break;
            }

            // Otherwise, add current cell to closed list and update g & h values of its neighbours
            openList.pop();
            closedList[index(currentCell)] = true;

            int gNew, hNew;
            for (const Cell& neighbour : GetNeighbours(currentCell, map))
            {
                const int neighbourIndex = index(neighbour);

                // Skip if already visited
                if (closedList[neighbourIndex])
                    continue;

                // Calculate scores
                gNew = false ? manhattan(neighbour, end) : euclidean(neighbour, end);
                hNew = Cost(GetType(neighbour, map));

                // Append if unvisited or best score
                if (tileNodes[neighbourIndex].f() == 0 || (gNew + hNew) < tileNodes[neighbourIndex].f())
                {
                    openList.push({ neighbour, gNew, hNew });
                    tileNodes[neighbourIndex] = { neighbour, currentCell, gNew, hNew };
                }
            }
        }
        nodes = tileNodes;

        // Generate path by traversing parents then inverting
        Path path;
        Cell currentCell = end;
        int currentIndex = index(currentCell);

        while (!(tileNodes[currentIndex].parent == currentCell))
        {
            path.push_back(currentCell);
            currentCell = tileNodes[currentIndex].parent;
            currentIndex = index(currentCell);
        }
        std::reverse(path.begin(), path.end());

        return path;
    }

    std::vector<Cell> GetNeighbours(const Cell& cell, const Map& map)
    {
        std::vector<Cell> cells;
        // Diagonals
        //for (int row = cell.row - 1; row <= cell.row + 1 && row >= 0 && row < MAP_SIZE; row++)
        //{
        //    for (int col = cell.col - 1; col <= cell.col + 1 && col >= 0 && col < MAP_SIZE; col++)
        //    {
        //        if (!(col == cell.col && row == cell.row))
        //            cells.push_back({ col, row });
        //    }
        //}
        if (cell.col - 1 >= 0) cells.push_back({ cell.col - 1, cell.row });
        if (cell.col + 1 < MAP_SIZE) cells.push_back({ cell.col + 1, cell.row });
        if (cell.row - 1 >= 0) cells.push_back({ cell.col, cell.row - 1 });
        if (cell.row + 1 < MAP_SIZE) cells.push_back({ cell.col, cell.row + 1 });
        return cells;
    }
}
