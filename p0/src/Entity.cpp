#include "Entity.h"
#include "Components.h"

std::unordered_map<Entity, uint32_t> gLookup;

Entity CreateEntity(Components& components)
{
	static Entity entity = 0;
	++entity;
	components.transforms.Add(entity).mSelf = entity;
	components.hierarchies.Add(entity);
	return entity;
}

Entity CreateEntity(Components& components, float x, float y, float z)
{
	Entity entity = CreateEntity(components);
	components.transforms.GetComponent(entity)->Translate(x, y, z);
	return entity;
}

void DestroyEntity(Entity entity, Components& components)
{
	auto destroy = [&](Entity e, Components& c) {
		assert(gLookup.find(e) != gLookup.end());
		c.hierarchies.Remove(e);
		c.transforms.Remove(e);
		c.identifiers.Remove(e);
		c.rigidbodies.Remove(e);
		c.colliders.Remove(e);
		c.buildings.Remove(e);
		c.players.Remove(e);
		gLookup.erase(e);
	};

	// Recurssive case
	for (Entity child : components.hierarchies.GetComponent(entity)->children)
		destroy(child, components);

	// Base case
	if (components.hierarchies.GetComponent(entity)->parent != INVALID_ENTITY)
		RemoveChild(components.hierarchies.GetComponent(entity)->parent, entity, components);
	destroy(entity, components);

	// Note that passing by reference to nullify the entity would create more problems than solutions.
	// Hierarchies store parents & children by-value, and changing Hierarchy to use pointers is also problematic.
	// Hence, the best we can do is erase from gLookup and nullify explicity via entity = INVALID_ENTITY;
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

Entity Root(Entity entity, Components& components)
{
	assert(entity != INVALID_ENTITY);
	Entity parent = components.hierarchies.GetComponent(entity)->parent;
	return parent == INVALID_ENTITY ? entity : Root(parent, components);
}

//EntityTransform& Get(Entity entity)
//{
//	return *EntityTransform::sComponents->transforms.GetComponent(entity);
//}
