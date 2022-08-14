#include "pch.h"
#include "Map.h"

/*
	Building Create(Type type)
	{
		
	}
*/

BuildingId Map::Add(Buildings::Type type, Collision::Colliders& colliders)
{
	BuildingId result;
	assert(type < Buildings::COUNT);
	result.type = type;
	UnorderedVector<Buildings::Building>& buildings = mBuildings[type];

	{
		Buildings::Building building;
		building.type = type;
		building.hitpoints = Buildings::Durability(type);
		// *Insert additional building information here*
		result.id = buildings.Add(std::move(building));
	}

	Vector3 bounds = Buildings::Bounds(type);

	//switch (type)
	//{
	//case Buildings::TD:
	//	break;
	//case Buildings::APARTMENT:
	//	break;
	//case Buildings::BMO:
	//	break;
	//case Buildings::CONDO:
	//	break;
	//case Buildings::DUPLEX:
	//	break;
	//case Buildings::OFFICE:
	//	break;
	//case Buildings::PINK:
	//	break;
	//case Buildings::PENTA:
	//	break;
	//};
	
	Buildings::Building* building = buildings.Get(result.id);
	colliders.Add(building->collider, bounds.y - bounds.x, bounds.x, Tags::BUILDING, building);
	return result;
}

void Map::Remove(BuildingId building)
{
	mBuildings[building.type].Remove(building.id);
}

Buildings::Building* Map::Get(BuildingId building)
{
	return mBuildings[building.type].Get(building.id);
}
