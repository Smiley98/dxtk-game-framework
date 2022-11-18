#pragma once
#include "Component.h"
#include "Collision2.h"
#include "Tags.h"

struct Components
{
	ComponentCollection<Transform3> transforms;
	ComponentCollection<Collision2::SphereCollider> spheres;
	ComponentCollection<Collision2::CapsuleCollider> capsules;
	ComponentCollection<Tags::Tag> tags;
};
