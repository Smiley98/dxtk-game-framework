#pragma once
#include "ComponentMap.h"
#include "Transform.h"
#include "Geometry.h"
#include "Kinematic.h"
#include "Building.h"
#include "Identifier.h"

using Transforms = ComponentMap<Transform>;
using Spheres = ComponentMap<Sphere>;
using Capsules = ComponentMap<Capsule>;
using Buildings = ComponentMap<Building>;
using Bodies = ComponentMap<Kinematic>;
using Identifiers = ComponentMap<Identifier>;

struct Components
{
	Transforms transforms;
	Spheres spheres;
	Capsules capsules;
	Bodies bodies;
	Buildings buildings;
	Identifiers identifiers;
};
