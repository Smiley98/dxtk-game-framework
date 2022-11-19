#include "PlayerFactory.h"
#include "Components.h"
#include "PlayerRenderer.h"

Entity CreatePlayer(Components& components, const PlayerRenderer& renderer)
{
	Entity entity = CreateEntity();
	components.tags.Add(entity) = Tags::PLAYER;
	components.transforms.Add(entity);

	Capsule& capsule = *ToCapsule(&components.colliders.Add(entity));
	AutoBound(capsule, renderer.Bounds());
	capsule.dynamic = true;
	capsule.sphere = false;

	return entity;
}
