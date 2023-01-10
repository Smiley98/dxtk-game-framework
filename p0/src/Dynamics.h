#pragma once
#include "Rigidbody.h"

// "Semi-implicit Euler integration" -- cheapest energy-conserving system
// v2 = v1 + a(t)
// p2 = p1 + v2(t) + 0.5a(t^2)

// Decelerate to zero (vf = 0) over distance d
// vf^2 = vi^2 + 2a(d)
// -vi^2 / 2d = a

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

    inline Vector3 Decelerate(
        const Vector3& targetPosition,
        const Vector3& seekerPosition,
        const Vector3& seekerVelocity)
    {
        float d = (targetPosition - seekerPosition).Length();
        float a = seekerVelocity.Dot(seekerVelocity) / (d * 2.0f);

        Vector3 v;
        seekerVelocity.Normalize(v);
        return -v * a;
    }
}
