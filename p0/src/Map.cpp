#include "pch.h"
#include "Map.h"

/*BuildingId Map::Add(Building::Type type, Collision::Colliders& colliders)
{
	BuildingId result;
	assert(type < Building::COUNT);
	result.type = type;
	UnorderedVector<Building>& buildings = mBuildings[type];

	{
		Building building;
		building.type = type;
		building.hitpoints = Building::Durability(type);
		// *Insert additional building information here*
		result.id = buildings.Add(std::move(building));
	}
	
	Vector3 bounds = Building::Bounds(type);
	Building* building = buildings.Get(result.id);
	Collision::CapsuleCollider* collider = nullptr;

	// Collider half height is either y or z depending on the building, hence the need for this switch.
	float radius = bounds.x;
	float halfHeight = bounds.y - radius;
	switch (type)
	{
	case Building::TD:
	case Building::BMO:
	case Building::CONDO:
	case Building::OFFICE:
	case Building::PENTA:
	case Building::PINK:
		halfHeight = bounds.z - radius;
		colliders.Add(building->collider, halfHeight, radius, Tags::BUILDING, building);
		collider = colliders.Get(building->collider);
		collider->Translate({ 0.0f, 0.0f, halfHeight });
		collider->SetPitch(90.0f);
		break;

	case Building::APARTMENT:
	case Building::DUPLEX:
		colliders.Add(building->collider, halfHeight, radius, Tags::BUILDING, building);
		break;
	};

	return result;
}*/

BuildingId Map::Add(Building::Type type, Collision2::Colliders& colliders)
{
	BuildingId result;
	assert(type < Building::COUNT);
	result.type = type;
	UnorderedVector<Building>& buildings = mBuildings[type];

	{
		Building building;
		building.type = type;
		building.hitpoints = Building::Durability(type);
		// *Insert additional building information here*
		result.id = buildings.Add(std::move(building));
	}

	Vector3 bounds = Building::Bounds(type);
	Building* building = buildings.Get(result.id);

	// Collider half height is either y or z depending on the building, hence the need for this switch.
	float radius = bounds.x;
	float halfHeight = bounds.y - radius;
	switch (type)
	{
	case Building::TD:
	case Building::BMO:
	case Building::CONDO:
	case Building::OFFICE:
	case Building::PENTA:
	case Building::PINK:
		halfHeight = bounds.z - radius;
		//building->transform.Translate({ 0.0f, 0.0f, halfHeight });
		//building->transform.RotateX(90.0f);
		break;

	case Building::APARTMENT:
	case Building::DUPLEX:
		break;

	default:
		assert(false);
	};

	// Buildings now own their Transform memory so we can call colliders.Add() outside of the switch statement!
	colliders.Add(building->collider, halfHeight, radius, &building->transform, Tags::BUILDING, building);
	return result;
}

void Map::Remove(BuildingId building)
{
	mBuildings[building.type].Remove(building.id);
}

Building* Map::Get(BuildingId building)
{
	return mBuildings[building.type].Get(building.id);
}

void Map::Render(const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
{
	for (size_t i = 0; i < mBuildings.size(); i++)
	{
		// DirectX::Model doesn't have a Draw() method for instanced rendering...
		//std::vector<Buildings::Building>& buildings = mBuildings[i].Objects();
		//if (!buildings.empty())
		//{
		//	Buildings::Type type = buildings.front().type;
		//	std::vector<Matrix> positions(buildings.size());
		//	for (const Buildings::Building& building : buildings)
		//	{
		//		positions.push_back(Matrix::CreateTranslation(building.position));
		//	}
		//}

		for (const Building& building : mBuildings[i].Objects())
		{
			Building::Draw(building, view, proj, graphics);
		}
	}
}

void MintyAftershave::Reset()
{
}
