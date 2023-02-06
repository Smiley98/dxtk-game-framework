#pragma once
#include <cstdint>

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
	}
}

using Entity = uint32_t;
static constexpr Entity INVALID_ENTITY = 0;

struct Components;

Entity CreateEntity(Components& components);
Entity CreateEntity(Components& components, const DirectX::SimpleMath::Vector3& position);
Entity CreateEntity(Components& components, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& direction);
Entity CreateEntity(Components& components, float x, float y, float z = 0.0f);
void DestroyEntity(Entity entity, Components& components);

void AddChild(Entity parent, Entity child, Components& components);
void RemoveChild(Entity parent, Entity child, Components& components);

Entity Root(Entity entity, Components& components);
