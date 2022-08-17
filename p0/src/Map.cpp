#include "pch.h"
#include "Map.h"

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
	case Buildings::CONDO:
	case Buildings::OFFICE:
	case Buildings::PENTA:
	case Buildings::PINK:
		halfHeight = bounds.z - radius;
		colliders.Add(building->collider, halfHeight, radius, Tags::BUILDING, building);
		collider = colliders.Get(building->collider);
		collider->Translate({ 0.0f, 0.0f, halfHeight });
		collider->Rotate({ 90.0f, 0.0f, 0.0f });
		break;

	case Buildings::APARTMENT:
	case Buildings::DUPLEX:
		colliders.Add(building->collider, halfHeight, radius, Tags::BUILDING, building);
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

		for (const Buildings::Building& building : mBuildings[i].Objects())
		{
			Buildings::Draw(building, view, proj, graphics);
		}
	}
}

void MintyAftershave::Reset()
{
}
