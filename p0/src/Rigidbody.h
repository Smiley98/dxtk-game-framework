#pragma once
#include "ComponentHash.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

// This is actually a "particle" as it has no mass, and only has physics-based linear (no angular) motion.
struct Rigidbody
{
	Vector3 velocity;
	Vector3 acceleration;
	//bool gravity = false;
	// No need for gravity yet

	static uint32_t Hash();
};
