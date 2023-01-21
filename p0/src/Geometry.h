#pragma once
#include "ComponentHash.h"

struct Geometry
{
	bool dynamic = false;	// no movement (static) vs movement (dynamic)
};

struct Sphere : public Geometry
{
	float r = 0.0f;			// radius

	static uint32_t Hash();
};

struct Capsule : public Geometry
{
	float r = 0.0f;			// radius
	float hh = 0.0f;		// half-height

	static uint32_t Hash();
};

struct Collider
{
	union
	{
		struct { float r; };
		struct { float r, hh; };
		struct { float x, y, z; };
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
