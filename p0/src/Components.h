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
	EntityTransform& GetTransform(Entity entity) { return *transforms.GetComponent(entity); }
	Hierarchy& GetHierarchy(Entity entity) { return *hierarchies.GetComponent(entity); }
	Identifier& GetIdentifier(Entity entity) { return *identifiers.GetComponent(entity); }

	// Physics Components
	ComponentMap<Rigidbody> rigidbodies;
	ComponentMap<Collider> colliders;
	Rigidbody& GetRigidbody(Entity entity) { return *rigidbodies.GetComponent(entity); }
	Collider& GetCollider(Entity entity) { return *colliders.GetComponent(entity); }

	// Gameplay Components
	ComponentMap<SteeringBehaviour> steering;
	ComponentMap<Building> buildings;
	ComponentMap<Player> players;
	SteeringBehaviour& GetSteering(Entity entity) { return *steering.GetComponent(entity); }
	Building& GetBuilding(Entity entity) { return *buildings.GetComponent(entity); }
	Player& GetPlayer(Entity entity) { return *players.GetComponent(entity); }
};
