#pragma once
#include "ComponentMap.h"
#include "Transform.h"
#include "Geometry.h"
#include "Kinematic.h"
#include "Building.h"
#include "Tags.h"

using Transforms = ComponentMap<Transform>;
using Spheres = ComponentMap<Sphere>;
using Capsules = ComponentMap<Capsule>;
using Buildings = ComponentMap<Building>;
using Bodies = ComponentMap<Kinematic>;

struct Components
{
	Transforms transforms;
	Spheres spheres;
	Capsules capsules;
	Bodies bodies;
	Buildings buildings;
	ComponentMap<Tags::Tag> tags;
};
