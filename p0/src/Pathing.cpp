#include "pch.h"
#include "Pathing.h"

using namespace Tile;
constexpr bool gManhattan = true;

Path FindPath(const Cell& start, const Cell& end, const Map& map)
{
    // Mark all nodes as unvisited (closed list = false) and append start to open list
    const int tileCount = MAP_SIZE * MAP_SIZE;
    std::vector<Node> tileNodes(tileCount);
    std::priority_queue<Node, std::vector<Node>, decltype(&Compare) > openList(Compare);
    std::vector<bool> closedList(tileCount, false);
    tileNodes[Index(start)].parent = start;
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
        closedList[Index(currentCell)] = true;

        float gNew, hNew;
        for (const Cell& neighbour : Neighbours(currentCell, map))
        {
            const int neighbourIndex = Index(neighbour);

            // Skip if already visited
            if (closedList[neighbourIndex])
                continue;

            // Calculate scores
            gNew = gManhattan ? Manhattan(neighbour, end) : Euclidean(neighbour, end);
            hNew = Cost(GetType(neighbour, map));

            // Append if unvisited or best score
            if (tileNodes[neighbourIndex].F() <= FLT_EPSILON ||
                (gNew + hNew) < tileNodes[neighbourIndex].F())
            {
                openList.push({ neighbour, gNew, hNew });
                tileNodes[neighbourIndex] = { neighbour, currentCell, gNew, hNew };
            }
        }
    }

    // Generate path by traversing parents then inverting
    Path path;
    Cell currentCell = end;
    int currentIndex = Index(currentCell);

    while (!(tileNodes[currentIndex].parent == currentCell))
    {
        path.push_back(currentCell);
        currentCell = tileNodes[currentIndex].parent;
        currentIndex = Index(currentCell);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}

Path FindPathDebug(const Cell& start, const Cell& end, int steps, const Map& map, std::vector<Node>& nodes)
{
    // Mark all nodes as unvisited (closed list = false) and append start to open list
    const int tileCount = MAP_SIZE * MAP_SIZE;
    std::vector<Node> tileNodes(tileCount);
    std::priority_queue<Node, std::vector<Node>, decltype(&Compare) > openList(Compare);
    std::vector<bool> closedList(tileCount, false);
    tileNodes[Index(start)].parent = start;
    openList.push({ start });

    for (int i = 0; i < steps; i++)
    {
        const Cell currentCell = openList.top().cell;

        // End condition (destination reached)
        if (currentCell == end)
        {
            break;
        }

        // Otherwise, add current cell to closed list and update g & h values of its neighbours
        openList.pop();
        closedList[Index(currentCell)] = true;

        float gNew, hNew;
        for (const Cell& neighbour : Neighbours(currentCell, map))
        {
            const int neighbourIndex = Index(neighbour);

            // Skip if already visited
            if (closedList[neighbourIndex])
                continue;

            // Calculate scores
            gNew = gManhattan ? Manhattan(neighbour, end) : Euclidean(neighbour, end);
            hNew = Cost(GetType(neighbour, map));

            // Append if unvisited or best score
            if (tileNodes[neighbourIndex].F() <= FLT_EPSILON ||
                (gNew + hNew) < tileNodes[neighbourIndex].F())
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
    int currentIndex = Index(currentCell);

    // If end doesn't have a parent, there's no solution
    if (tileNodes[currentIndex].parent == Cell{ -1, -1 })
        return {};

    while (!(tileNodes[currentIndex].parent == currentCell))
    {
        path.push_back(currentCell);
        currentCell = tileNodes[currentIndex].parent;
        currentIndex = Index(currentCell);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return path;
}
