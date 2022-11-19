#include "BuildingFactory.h"
#include "Components.h"
#include "BuildingRenderer.h"
#include "Utility.h"

Entity CreateBuilding(Components& components,
	Building::Type type, const BuildingRenderer& renderer)
{
	Entity entity = CreateEntity();
	components.tags.Add(entity) = Tags::BUILDING;
	components.transforms.Add(entity);

	Capsule& capsule = *ToCapsule(&components.colliders.Add(entity));
	AutoBound(capsule, renderer.Bounds(type));
	capsule.dynamic = false;
	capsule.sphere = false;

	Building& building = components.buildings.Add(entity);
	building.durability = renderer.MaxDurability(type);
	building.type = type;

	return entity;
}
