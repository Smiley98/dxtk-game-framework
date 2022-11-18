#include "PlayerFactory.h"
#include "Components.h"
#include "PlayerRenderer.h"

Entity CreatePlayer(Components& components, const PlayerRenderer& renderer)
{
	Entity entity = CreateEntity();
	components.tags.Add(entity) = Tags::PLAYER;
	components.transforms.Add(entity);

	Collision2::CapsuleCollider& capsule = components.capsules.Add(entity);
	capsule.AutoBound(renderer.Bounds());
	capsule.dynamic = true;

	return entity;
}
