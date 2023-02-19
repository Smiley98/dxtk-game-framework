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
	EntityTransform& GetTransform(Entity entity) { return *transforms.Get(entity); }
	Hierarchy& GetHierarchy(Entity entity) { return *hierarchies.Get(entity); }
	Identifier& GetIdentifier(Entity entity) { return *identifiers.Get(entity); }

	// Physics Components
	ComponentMap<Rigidbody> rigidbodies;
	ComponentMap<Collider> colliders;
	Rigidbody& GetRigidbody(Entity entity) { return *rigidbodies.Get(entity); }
	Collider& GetCollider(Entity entity) { return *colliders.Get(entity); }

	// Gameplay Components
	ComponentMap<SteeringBehaviour> steering;
	ComponentMap<Building> buildings;
	ComponentMap<Player> players;
	SteeringBehaviour& GetSteering(Entity entity) { return *steering.Get(entity); }
	Building& GetBuilding(Entity entity) { return *buildings.Get(entity); }
	Player& GetPlayer(Entity entity) { return *players.Get(entity); }
};
