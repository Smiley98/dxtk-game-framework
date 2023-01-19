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