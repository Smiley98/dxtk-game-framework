#include "PlayerFactory.h"
#include "Component.h"
#include "Collision2.h"

Entity CreatePlayer(
	ComponentCollection<Tags::Tag>& tags,
	ComponentCollection<Transform3>& transforms,
	ComponentCollection<Collision2::CapsuleCollider>& capsules,
	float radius, float halfHeight
)
{
	Entity entity = CreateEntity();
	tags.Add(entity) = Tags::PLAYER;
	Transform3& transform = transforms.Add(entity);

	Collision2::CapsuleCollider& capsule = capsules.Add(entity);
	capsule.entity = entity;
	capsule.transform = &transform;
	capsule.radius = radius;
	capsule.halfHeight = halfHeight;

	return entity;
}
