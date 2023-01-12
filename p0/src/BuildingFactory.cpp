#include "BuildingFactory.h"
#include "BuildingRenderer.h"
#include "Components.h"
#include "Utility.h"

Entity CreateBuilding(Components& components,
	Building::Type type, const BuildingRenderer& renderer)
{
	Entity entity = CreateEntity();
	components.identifiers.Add(entity).tag = Tags::BUILDING;

	Transform& transform = components.transforms.Add(entity);
	Capsule& capsule = components.capsules.Add(entity);
	Vector3 bounds = renderer.Bounds(type);
	switch (type)
	{
		case Building::TD:
		case Building::BMO:
		case Building::PENTA:
		{
			float r = bounds.x;
			float hh = bounds.z - r;
			capsule.r = r;
			capsule.hh = hh;
			break;
		}

		case Building::APARTMENT:
		case Building::CONDO:
		case Building::DUPLEX:
		case Building::OFFICE:
		case Building::PINK:
		{
			float r = bounds.x;
			float hh = bounds.y - r;
			capsule.r = r;
			capsule.hh = hh;
			break;
		}
	}
	capsule.dynamic = false;
	// Historically used AutoBound(capsule, renderer.Bounds(type));
	// TD, BMO and PENTA are Z_FORWARDS instead of Y_FORWARDS so calculating all bounds inline.

	Building& building = components.buildings.Add(entity);
	building.durability = renderer.MaxDurability(type);
	building.type = type;

	return entity;
}
