#pragma once
#include "Component.h"
#include "Transform.h"
#include "Building.h"
#include "Geometry.h"
#include "Tags.h"

struct Components
{
	ComponentCollection<Transform> transforms;
	ComponentCollection<Sphere> spheres;
	ComponentCollection<Capsule> capsules;
	ComponentCollection<Building> buildings;
	ComponentCollection<Tags::Tag> tags;
};
