#include "pch.h"
#include "BuildingFactory.h"
#include "BuildingRenderer.h"
#include "Components.h"
#include "Utility.h"

Entity CreateBuilding(Components& components,
	Building::Type type, const BuildingRenderer& renderer)
{
	Entity entity = CreateEntity(components);
	Entity collider = CreateEntity(components);
	AddChild(entity, collider, components);
	components.identifiers.Add(collider).tag = Tags::BUILDING;

	EntityTransform& transform = *components.transforms.GetComponent(collider);
	Capsule& capsule = components.capsules.Add(collider);
	Vector3 bounds = renderer.Bounds(type);
	switch (type)
	{
		case Building::TD:
		case Building::BMO:
		case Building::PENTA:
		case Building::PINK:
		{
			BoundZ(capsule, bounds);
			//transform.TranslateZ(capsule.hh);
			transform.RotateX(90.0f);
			break;
		}

		// TODO -- add multiple colliders for apartment and office.
		case Building::APARTMENT:
		case Building::CONDO:
		case Building::DUPLEX:
		case Building::OFFICE:
		{
			BoundY(capsule, bounds);
			break;
		}
	}
	capsule.dynamic = false;

	Building& building = components.buildings.Add(entity);
	building.durability = renderer.MaxDurability(type);
	building.type = type;

	return entity;
}
