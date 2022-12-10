#pragma once
#include "SimpleMath.h"
#include "ComponentHash.h"

struct Kinematic
{
	float hVel = 0.0f;	// horizontal velocity
	float hAcc = 0.0f;	// horizontal acceleration

	// Didn't want the awkwardness of separate HorizontalMotion and VerticalMotion
	// components even though grenades are the only object anticipated to have vertical motion.
	//float vVel = 0.0f;	// vertical velocity
	//float vAcc = 0.0f;	// vertical acceleration
	//bool gravity = false;

	static uint32_t Hash();
};
