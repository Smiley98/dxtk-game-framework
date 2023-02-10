#include "pch.h"
#include "Pathing.h"

namespace Pathing {
    using namespace Tile;
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

        void Print()
        {
            printf("Cell {%i,%i}: f = %i (%ig + %ih)\n", cell.col, cell.row, f(), g, h);
        }

        Cell cell, parent;
        int g, h;
    };

    Path FindPath(const Cell& start, const Cell& end, const Map& map)
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
                gNew = true ? manhattan(neighbour, end) : euclidean(neighbour, end);
                hNew = Cost(GetType(neighbour, map));

                // Append if unvisited or best score
                if (tileNodes[neighbourIndex].f() == 0 || (gNew + hNew) < tileNodes[neighbourIndex].f())
                {
                    openList.push({ neighbour, gNew, hNew });
                    tileNodes[neighbourIndex] = { neighbour, currentCell, gNew, hNew };
                }
            }
        }

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
        for (int row = cell.row - 1; row <= cell.row + 1 && row >= 0 && row < MAP_SIZE; row++)
        {
            for (int col = cell.col - 1; col <= cell.col + 1 && col >= 0 && col < MAP_SIZE; col++)
            {
                if (!(col == cell.col && row == cell.row))
                    cells.push_back({ col, row });
            }
        }
        return cells;
    }
}
//*/
