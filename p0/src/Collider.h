#pragma once
#include "ComponentHash.h"
#include "Entity.h"

struct Components;
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
		NONE,
		SPHERE,
		CAPSULE,
		BOX
	} type = NONE;

	bool dynamic = false;

	static uint32_t Hash();
};

void AddSphere(Entity entity, float r, Components& components);
void AddCapsule(Entity entity, float r, float hh, Components& components);

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
	}
}

void BoundY(Collider& collider, const DirectX::SimpleMath::Vector3& bounds);
void BoundZ(Collider& collider, const DirectX::SimpleMath::Vector3& bounds);
