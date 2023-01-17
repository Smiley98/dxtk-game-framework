#pragma once
#include "ComponentHash.h"
#include "Entity.h"

struct SteeringBehaviour
{
	enum Type : size_t
	{
		NONE,
		SEEK,
		FLEE,
		PURSUE,
		EVADE,
		ARRIVE,
		AVOID
	} type;

	Entity target = INVALID_ENTITY;
	float maxSpeed = 0.0f;

	static uint32_t Hash();
};
