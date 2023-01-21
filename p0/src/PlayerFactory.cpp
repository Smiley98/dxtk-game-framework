#include "pch.h"
#include "PlayerFactory.h"
#include "PlayerRenderer.h"
#include "Components.h"

Entity CreatePlayer(Components& components, const PlayerRenderer& renderer)
{
	Entity entity = CreateEntity(components);
	components.identifiers.Add(entity).tag = Tags::PLAYER;
	components.rigidbodies.Add(entity);

	Player& player = components.players.Add(entity);
	player.steering = 250.0f;
	player.type = Player::VAN;

	Collider& collider = components.colliders.Add(entity);
	collider.type = Collider::CAPSULE;
	collider.dynamic = true;
	BoundY(collider, renderer.Bounds());

	return entity;
}
