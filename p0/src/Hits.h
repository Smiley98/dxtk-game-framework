#pragma once
#include "Entity.h"
#include "SimpleMath.h"
#include <array>

struct Hit
{
	Entity target = INVALID_ENTITY;
	DirectX::SimpleMath::Vector3 mtv;
};

struct HitPair
{
	std::array<Entity, 2> hits{ INVALID_ENTITY, INVALID_ENTITY };
	DirectX::SimpleMath::Vector3 mtv;
};
