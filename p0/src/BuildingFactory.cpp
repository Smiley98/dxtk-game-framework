#include "pch.h"
#include "BuildingFactory.h"
#include "BuildingRenderer.h"
#include "Components.h"
#include "Utility.h"

Entity CreateBuilding(Components& components,
	Building::Type type, const BuildingRenderer& renderer)
{
	Entity entity = CreateEntity();
	Entity collider = CreateEntity();
	components.transforms.Add(entity);
	components.transforms.Add(collider).parent = entity;
	components.identifiers.Add(collider).tag = Tags::BUILDING;
	// Collision resolver needs tags to function, so adding tag to collider instead of building.
	// Update: Resolve now fails during building damage callback... Need to make Hierarchy component!

	EntityTransform& transform = *components.transforms.GetComponent(collider);
	Capsule& capsule = components.capsules.Add(collider);
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
			// Should be bounds.z but that pushes us under since no ground plane
			transform.TranslateZ(hh);
			transform.RotateX(90.0f);
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
			// TODO -- fix getting pushed under buildings
			//transform.TranslateZ(r);
			break;
		}
	}
	capsule.dynamic = false;
	// Historically used AutoBound(capsule, renderer.Bounds(type));
	// TD, BMO and PENTA are Z_FORWARDS instead of Y_FORWARDS so calculating all bounds inline.

	Building& building = components.buildings.Add(entity);
	building.durability = renderer.MaxDurability(type);
	building.type = type;
	building.collider = collider;

	return entity;
}
