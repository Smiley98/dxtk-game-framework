#pragma once

struct Geometry
{
	float r = 0.0f;			// radius
	float hh = 0.0f;		// half-height (unused if sphere)
	bool sphere = false;	// sphere vs capsule (this is to avoid oop dynamic dispatch)
	bool dynamic = false;	// no movement (static) vs movement (dynamic)
};

struct Sphere : public Geometry {};
struct Capsule : public Geometry {};

inline const Sphere* ToSphere(const Geometry* geometry)
{
	return static_cast<const Sphere*>(geometry);
}

inline Sphere* ToSphere(Geometry* geometry)
{
	return static_cast<Sphere*>(geometry);
}

inline const Capsule* ToCapsule(const Geometry* geometry)
{
	return static_cast<const Capsule*>(geometry);
}

inline Capsule* ToCapsule(Geometry* geometry)
{
	return static_cast<Capsule*>(geometry);
}

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
	}
}

void AutoBound(Capsule& capsule, const DirectX::SimpleMath::Vector3& bounds);
