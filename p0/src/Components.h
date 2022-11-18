#pragma once
#include "Component.h"
#include "Collision2.h"
#include "Building.h"
#include "Tags.h"

struct Components
{
	ComponentCollection<Transform3> transforms;
	ComponentCollection<Collision2::SphereCollider> spheres;
	ComponentCollection<Collision2::CapsuleCollider> capsules;
	ComponentCollection<Building> buildings;
	ComponentCollection<Tags::Tag> tags;
};
