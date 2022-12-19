#pragma once
#include "Transform.h"

namespace Steering
{
    //public static Vector3 Seek(Vector3 target, Rigidbody current, float speed)
    //{
    //    Vector3 desiredVelocity = (target - current.position).normalized * speed;
    //    Vector3 currentVelocity = current.velocity;
    //    return desiredVelocity - currentVelocity;
    //}

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
}