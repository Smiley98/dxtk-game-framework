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
	Buildings::Building* building = buildings.Get(result.id);
	Collision::CapsuleCollider* collider = nullptr;

	// Collider half height is either y or z depending on the building, hence the need for this switch.
	float radius = bounds.x;
	float halfHeight = bounds.y - radius;
	switch (type)
	{
	case Buildings::TD:
	case Buildings::BMO:
		halfHeight = bounds.z - radius;
		colliders.Add(building->collider, halfHeight, radius, Tags::BUILDING, building);
		collider = colliders.Get(building->collider);
		collider->Translate({ 0.0f, 0.0f, halfHeight });
		collider->Rotate({ 90.0f, 0.0f, 0.0f });
		break;
	case Buildings::APARTMENT:
		colliders.Add(building->collider, halfHeight, radius, Tags::BUILDING, building);
		break;
	case Buildings::CONDO:
		break;
	case Buildings::DUPLEX:
		break;
	case Buildings::OFFICE:
		break;
	case Buildings::PINK:
		break;
	case Buildings::PENTA:
		break;
	};

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

void MintyAftershave::Load()
{
}

void MintyAftershave::Unload()
{
}
