#include "PlayerFactory.h"
#include "Components.h"
#include "PlayerRenderer.h"

Entity CreatePlayer(Components& components, const PlayerRenderer& renderer)
{
	Entity entity = CreateEntity();
	components.tags.Add(entity) = Tags::PLAYER;

	Collision2::CapsuleCollider& capsule = components.capsules.Add(entity);
	capsule.entity = entity;
	capsule.transform = &components.transforms.Add(entity);
	capsule.AutoBound(renderer.Bounds());

	return entity;
}
