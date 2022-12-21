#pragma once
#include "SimpleMath.h"
#include "ComponentHash.h"

struct Kinematic
{
	float vel = 0.0f;
	float acc = 0.0f;

	static uint32_t Hash();
};
