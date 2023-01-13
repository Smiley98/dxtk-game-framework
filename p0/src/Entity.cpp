#include "Entity.h"
#include "Components.h"

Entity CreateEntity()
{
	static Entity entity = 0;
	return ++entity;
}

void DestroyEntity(Entity& entity, Components& components)
{
	// TODO -- Destroy children on parent destruction.
	components.transforms.Remove(entity);
	components.hierarchies.Remove(entity);
	components.identifiers.Remove(entity);
	components.rigidbodies.Remove(entity);
	components.spheres.Remove(entity);
	components.capsules.Remove(entity);
	components.buildings.Remove(entity);
	components.players.Remove(entity);
	entity = INVALID_ENTITY;
}

// Pass INVALID_ENTITY to nullify
void SetParent(Entity parent, Entity child, Components& components)
{
	if (components.hierarchies.GetComponent(child) == nullptr)
		components.hierarchies.Add(child).parent = parent;
	else
		components.hierarchies.GetComponent(child)->parent = parent;
}
