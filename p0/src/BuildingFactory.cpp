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

	// push_back causes a reallocation which is why the pointer is getting invalidated!
	// Collider no longer needs to know its entity for collision response since we now must loop through all entities!
	//capsule.entity = entity;
	//capsule.transform = &transform;
	// Switching from vector to array won't save us because component removal would still mess up addresses xD

	Collision2::CapsuleCollider& capsule = components.capsules.Add(entity);
	capsule.AutoBound(renderer.Bounds(type));
	capsule.dynamic = false;

	Building& building = components.buildings.Add(entity);
	building.durability = renderer.MaxDurability(type);
	building.type = type;

	return entity;
}
