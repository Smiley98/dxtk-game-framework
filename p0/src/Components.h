#pragma once
#include "ComponentMap.h"

#include "EntityTransform.h"
#include "Hierarchy.h"
#include "Identifier.h"

#include "Rigidbody.h"
#include "Collider.h"

#include "SteeringBehaviour.h"
#include "Building.h"
#include "Player.h"

struct Components
{
	// Foundational Components
	ComponentMap<EntityTransform> transforms;
	ComponentMap<Hierarchy> hierarchies;
	ComponentMap<Identifier> identifiers;

	// Physics Components
	ComponentMap<Rigidbody> rigidbodies;
	ComponentMap<Collider> colliders;

	// Gameplay Components
	ComponentMap<SteeringBehaviour> steering;
	ComponentMap<Building> buildings;
	ComponentMap<Player> players;
};
