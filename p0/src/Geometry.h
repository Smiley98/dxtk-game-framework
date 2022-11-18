#pragma once

struct Geometry
{
	bool dynamic = false;	// no movement (static) vs movement (dynamic)
};

struct Sphere : public Geometry
{
	float r = 0.0f;		// radius
};

struct Capsule : public Geometry
{
	float r = 0.0f;		// radius
	float hh = 0.0f;	// half-height
};

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
	}
}

void AutoBound(Capsule& capsule, const DirectX::SimpleMath::Vector3& bounds);
