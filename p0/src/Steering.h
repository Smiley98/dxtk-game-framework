#pragma once
#include "Transform.h"
#include "Dynamics.h"

namespace Steering
{
	inline Vector3 Seek(
        const Vector3& targetPosition,
        const Vector3& seekerPosition,
        const Vector3& seekerVelocity, float maxSpeed)
	{
        Vector3 desiredVelocity;
        (targetPosition - seekerPosition).Normalize(desiredVelocity);
        desiredVelocity *= maxSpeed;

        return desiredVelocity - seekerVelocity;
	}

    inline Vector3 Flee(
        const Vector3& targetPosition,
        const Vector3& seekerPosition,
        const Vector3& seekerVelocity, float maxSpeed)
    {
        return -Seek(targetPosition, seekerPosition, seekerVelocity, maxSpeed);
    }

    inline Vector3 Pursue(
        const Vector3& targetPosition,
        const Vector3& targetVelocity,
        const Vector3& targetAcceleration,
        const Vector3& seekerPosition,
        const Vector3& seekerVelocity, float maxSpeed, float dt)
    {
        return Seek(
            targetPosition + Dynamics::Integrate(targetVelocity, targetAcceleration, dt),
            seekerPosition, seekerVelocity, maxSpeed);
    }

    inline Vector3 Evade(
        const Vector3& targetPosition,
        const Vector3& targetVelocity,
        const Vector3& targetAcceleration,
        const Vector3& seekerPosition,
        const Vector3& seekerVelocity, float maxSpeed, float dt)
    {
        return -Seek(
            targetPosition + Dynamics::Integrate(targetVelocity, targetAcceleration, dt),
            seekerPosition, seekerVelocity, maxSpeed);
    }

    inline Vector3 Wander(
        const Vector3& seekerPosition,
        const Vector3& seekerVelocity,
        float maxSpeed, float radius)
    {
        Vector3 direction;
        seekerVelocity.Normalize(direction);
        Vector3 centre = seekerPosition + direction * radius;
        Vector3 offset = RandomSpherePoint(radius);
        return Seek(centre + offset, seekerPosition, seekerVelocity, maxSpeed);
    }
}
