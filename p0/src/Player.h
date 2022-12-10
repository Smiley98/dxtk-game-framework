#pragma once
#include "SimpleMath.h"

struct Player
{
	enum Type
	{
		VAN,
		MECH
	} type;

	// Angular velocity -- Player is the only object that rotates based on physics.
	// (Not calculated as a true angular velocity - its a constant based on equipment).
	float steering = 0.0f;

	static uint32_t Hash();
};
