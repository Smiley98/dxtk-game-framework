#pragma once
#include "Transform.h"

// MTV resolves B from A where t = translation and r = radius
inline bool SphereSphere(const DirectX::SimpleMath::Vector3& tA, float rA, const DirectX::SimpleMath::Vector3& tB, float rB, DirectX::SimpleMath::Vector3& mtv)
{
	using namespace DirectX::SimpleMath;
	Vector3 AB = tB - tA;
	float lengthAB = AB.Length();
	float radiiAB = rA + rB;
	bool colliding = lengthAB <= radiiAB;
	if (colliding)
	{
		constexpr float resolution = 1.0f + FLT_EPSILON * 16.0f;
		Vector3 direction;
		AB.Normalize(direction);
		mtv = resolution * direction * (radiiAB - lengthAB);
	}
	return colliding;
}
