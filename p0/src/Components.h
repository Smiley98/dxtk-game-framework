#pragma once
#include "ComponentMap.h"
#include "EntityTransform.h"
#include "Rigidbody.h"
#include "Geometry.h"
#include "Building.h"
#include "Identifier.h"
#include "Player.h"

struct Components
{
	ComponentMap<Transform> transforms;
	ComponentMap<Rigidbody> rigidbodies;
	ComponentMap<Sphere> spheres;
	ComponentMap<Capsule> capsules;
	ComponentMap<Building> buildings;
	ComponentMap<Identifier> identifiers;
	ComponentMap<Player> players;
};
