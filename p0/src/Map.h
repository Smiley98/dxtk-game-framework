#pragma once
#include "Buildings.h"
#include <array>

struct BuildingId
{
	Id id;
	Buildings::Type type;
};

class Map
{
public:
	BuildingId Add(Buildings::Type type, Collision::Colliders& colliders);
	void Remove(BuildingId building);
	Buildings::Building* Get(BuildingId building);

private:
	std::array<UnorderedVector<Buildings::Building>, Buildings::COUNT> mBuildings;
};
