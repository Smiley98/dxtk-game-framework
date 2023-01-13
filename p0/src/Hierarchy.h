#pragma once
#include "ComponentHash.h"
#include "Entity.h"
#include <unordered_set>

struct Hierarchy
{
	static uint32_t Hash();
	Entity parent = INVALID_ENTITY;
	std::unordered_set<Entity> children;
};
