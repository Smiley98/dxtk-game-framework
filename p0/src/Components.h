#pragma once
#include "Component.h"
#include "Transform.h"
#include "Building.h"
#include "Geometry.h"
#include "Tags.h"

using TransformCollection = ComponentCollection<Transform>;
using SphereCollection = ComponentCollection<Sphere>;
using CapsuleCollection = ComponentCollection<Capsule>;
using BuildingCollection = ComponentCollection<Building>;
using TagCollection = ComponentCollection<Tags::Tag>;

struct Components
{
	TransformCollection transforms;
	SphereCollection spheres;
	CapsuleCollection capsules;
	BuildingCollection buildings;
	TagCollection tags;
};
