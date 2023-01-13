#pragma once
#include "ComponentMap.h"
#include "EntityTransform.h"
#include "Hierarchy.h"
#include "Identifier.h"
#include "Rigidbody.h"
#include "Geometry.h"
#include "Building.h"
#include "Player.h"

struct Components
{
	ComponentMap<EntityTransform> transforms;
	ComponentMap<Hierarchy> hierarchies;
	ComponentMap<Identifier> identifiers;
	ComponentMap<Rigidbody> rigidbodies;
	ComponentMap<Sphere> spheres;
	ComponentMap<Capsule> capsules;
	ComponentMap<Building> buildings;
	ComponentMap<Player> players;
};
