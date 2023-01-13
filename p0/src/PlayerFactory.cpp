#include "pch.h"
#include "PlayerFactory.h"
#include "Components.h"
#include "PlayerRenderer.h"
#include "Constants.h"

Entity CreatePlayer(Components& components, const PlayerRenderer& renderer)
{
	Entity entity = CreateEntity(components);
	components.identifiers.Add(entity).tag = Tags::PLAYER;
	components.rigidbodies.Add(entity);

	Player& player = components.players.Add(entity);
	player.steering = 250.0f;
	player.type = Player::VAN;

	Capsule& capsule = components.capsules.Add(entity);
	AutoBound(capsule, renderer.Bounds());
	capsule.dynamic = true;

	return entity;
}
