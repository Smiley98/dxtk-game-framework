#include "PlayerFactory.h"
#include "Components.h"
#include "PlayerRenderer.h"
#include "Constants.h"

Entity CreatePlayer(Components& components, const PlayerRenderer& renderer)
{
	Entity entity = CreateEntity();
	Player& player = components.players.Add(entity);
	player.steering = Constants::VAN_STEERING_MAX;
	player.type = Player::VAN;
	components.identifiers.Add(entity).tag = Tags::PLAYER;

	components.transforms.Add(entity);
	components.bodies.Add(entity);

	Capsule& capsule = components.capsules.Add(entity);
	AutoBound(capsule, renderer.Bounds());
	capsule.dynamic = true;

	return entity;
}
