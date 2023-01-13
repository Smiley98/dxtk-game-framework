#pragma once
#include "ComponentHash.h"
#include "Entity.h"

struct Hierarchy
{
	static uint32_t Hash();
	Entity parent = INVALID_ENTITY;
};
