#pragma once
#include "Component.h"
#include "Transform3.h"
#include "Building.h"
#include "Geometry.h"
#include "Tags.h"

struct Components
{
	ComponentCollection<Transform3> transforms;
	ComponentCollection<Geometry> colliders;
	ComponentCollection<Building> buildings;
	ComponentCollection<Tags::Tag> tags;
};
