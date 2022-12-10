#pragma once
#include "ComponentMap.h"
#include "Transform.h"
#include "Geometry.h"
#include "Kinematic.h"
#include "Building.h"
#include "Identifier.h"
#include "Player.h"

struct Components
{
	ComponentMap<Transform> transforms;
	ComponentMap<Sphere> spheres;
	ComponentMap<Capsule> capsules;
	ComponentMap<Kinematic> bodies;
	ComponentMap<Building> buildings;
	ComponentMap<Identifier> identifiers;
	ComponentMap<Player> players;
};
