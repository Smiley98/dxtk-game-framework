#pragma once
#include <cstdint>

using Entity = uint32_t;
static constexpr Entity INVALID_ENTITY = 0;

struct Components;

Entity CreateEntity(Components& components);
void DestroyEntity(Entity entity, Components& components);

void AddChild(Entity parent, Entity child, Components& components);
void RemoveChild(Entity parent, Entity child, Components& components);

Entity Root(Entity entity, Components& components);

// Even this is a little awkward because I can't make an informative name that's short.
//struct EntityTransform;
//EntityTransform& Get(Entity entity);

// Probably not a good idea to add a static component* to other component classes.
// EntityTransform needs its static reference because of FK,
// but its not necessary elsewhere and statics make lifetimes confusing.
//struct Collider;
//Collider& Collider(Entity entity);
