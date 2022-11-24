#pragma once
#include "Entity.h"
#include <array>

struct HitPair
{
	std::array<Entity, 2> hits{ INVALID_ENTITY, INVALID_ENTITY };
	Vector3 mtv;
};
