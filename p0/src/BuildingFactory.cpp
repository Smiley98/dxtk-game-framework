#include "BuildingFactory.h"
#include "Components.h"
#include "BuildingRenderer.h"

Entity CreateBuilding(Components& components,
	Building::Type type, const BuildingRenderer& renderer)
{
	Entity entity = CreateEntity();
	components.tags.Add(entity) = Tags::BUILDING;

	Collision2::CapsuleCollider& capsule = components.capsules.Add(entity);
	capsule.entity = entity;
	capsule.transform = &components.transforms.Add(entity);
	capsule.AutoBound(renderer.Bounds(type));

	Building& building = components.buildings.Add(entity);
	building.durability = renderer.MaxDurability(type);
	building.type = type;

	return entity;
}
