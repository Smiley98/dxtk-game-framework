#pragma once
#include "ComponentHash.h"

struct Collider
{
	union
	{
		struct { float r; };		// sphere radius
		struct { float r, hh; };	// capsule radius and half-height
		struct { float x, y, z; };	// box extents (half-bounds)
	};

	enum
	{
		SPHERE,
		CAPSULE,
		BOX
	} type;

	bool dynamic = false;

	static uint32_t Hash();
};

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
	}
}

void BoundY(Collider& collider, const DirectX::SimpleMath::Vector3& bounds);
void BoundZ(Collider& collider, const DirectX::SimpleMath::Vector3& bounds);
