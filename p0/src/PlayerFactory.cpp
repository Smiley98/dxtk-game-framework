#include "PlayerFactory.h"
#include "Components.h"
#include "PlayerRenderer.h"

Entity CreatePlayer(Components& components, const PlayerRenderer& renderer)
{
	Entity entity = CreateEntity();
	components.tags.Add(entity) = Tags::PLAYER;
	components.transforms.Add(entity);

	Collision2::Capsule& capsule = components.capsules.Add(entity);
	Collision2::AutoBound(capsule, renderer.Bounds());
	capsule.dynamic = true;

	return entity;
}
