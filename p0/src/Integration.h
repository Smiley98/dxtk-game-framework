#pragma once
#include "Rigidbody.h"

// "Semi-implicit Euler integration" -- cheapest energy-conserving system
// v2 = v1 + a(t)
// p2 = p1 + v2(t) + 0.5a(t^2)

namespace Dynamics
{
	inline Vector3 Integrate(const Vector3& vel, const Vector3& acc, float dt)
	{
		return vel * dt + 0.5f * acc * dt * dt;
	}

	inline float Integrate(float vel, float acc, float dt)
	{
		return vel * dt + 0.5f * acc * dt * dt;
	}
}
