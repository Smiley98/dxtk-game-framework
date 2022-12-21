#pragma once
#include "ComponentMap.h"
#include "Transform.h"
#include "Geometry.h"
#include "Kinematic.h"
#include "Kinematic3.h"
#include "Building.h"
#include "Identifier.h"
#include "Player.h"

struct Components
{
	ComponentMap<Transform> transforms;
	ComponentMap<Sphere> spheres;
	ComponentMap<Capsule> capsules;
	ComponentMap<Kinematic> bodies;
	ComponentMap<Kinematic3> bodies3;
	ComponentMap<Building> buildings;
	ComponentMap<Identifier> identifiers;
	ComponentMap<Player> players;
};
