#include "PlayerFactory.h"
#include "Components.h"

Entity CreatePlayer(Components& components, float radius, float halfHeight)
{
	Entity entity = CreateEntity();
	components.tags.Add(entity) = Tags::PLAYER;
	Transform3& transform = components.transforms.Add(entity);

	Collision2::CapsuleCollider& capsule = components.capsules.Add(entity);
	capsule.entity = entity;
	capsule.transform = &transform;
	capsule.radius = radius;
	capsule.halfHeight = halfHeight;

	return entity;
}
