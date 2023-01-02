#pragma once
#include "SimpleMath.h"

struct Player
{
	enum Type
	{
		VAN,
		MECH
	} type;

	float vel = 0.0f, acc = 0.0f;
	float steering = 0.0f; // Constant angular velocity

	static uint32_t Hash();
};
