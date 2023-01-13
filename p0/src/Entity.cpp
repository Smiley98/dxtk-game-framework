#include "Entity.h"
#include "Components.h"

Entity CreateEntity(Components& components)
{
	static Entity entity = 0;
	++entity;
	components.transforms.Add(entity).mSelf = entity;
	components.hierarchies.Add(entity);
	return entity;
}

void DestroyEntity(Entity& entity, Components& components)
{
	// TODO -- Destroy children on parent destruction.
	Hierarchy& childHierarchy = *components.hierarchies.GetComponent(entity);

	components.hierarchies.Remove(entity);
	components.transforms.Remove(entity);
	components.identifiers.Remove(entity);
	components.rigidbodies.Remove(entity);
	components.spheres.Remove(entity);
	components.capsules.Remove(entity);
	components.buildings.Remove(entity);
	components.players.Remove(entity);
	entity = INVALID_ENTITY;
}

void AddChild(Entity parent, Entity child, Components& components)
{
	if (components.hierarchies.GetComponent(child)->parent != INVALID_ENTITY)
	{
		components.hierarchies.GetComponent(
			components.hierarchies.GetComponent(child)->parent
		)->children.erase(child);
	}

	components.hierarchies.GetComponent(child)->parent = parent;
	components.hierarchies.GetComponent(parent)->children.insert(child);
}

void RemoveChild(Entity parent, Entity child, Components& components)
{
	assert(
		components.hierarchies.GetComponent(child)->parent == parent &&
		components.hierarchies.GetComponent(parent)->children.find(child) !=
		components.hierarchies.GetComponent(parent)->children.end()
	);

	components.hierarchies.GetComponent(parent)->children.erase(child);
	components.hierarchies.GetComponent(child)->parent = INVALID_ENTITY;
}
