#pragma once
#include "Transform.h"

inline void Bounds(const RigidTransform& t, float hh, DirectX::SimpleMath::Vector3& upper, DirectX::SimpleMath::Vector3& lower);
inline DirectX::SimpleMath::Vector3 NearestPoint(const DirectX::SimpleMath::Vector3& a, const DirectX::SimpleMath::Vector3& b, const DirectX::SimpleMath::Vector3& p);
inline void NearestSpheres(
	const RigidTransform& tA, float hhA, float rA,
	const RigidTransform& tB, float hhB, float rB,
	DirectX::SimpleMath::Vector3& bestA,
	DirectX::SimpleMath::Vector3& bestB);

// t = translation, r = radius
inline bool SphereSphere(const DirectX::SimpleMath::Vector3& tA, float rA, const DirectX::SimpleMath::Vector3& tB, float rB)
{
	using namespace DirectX::SimpleMath;
	Vector3 AB = tB - tA;
	float lengthAB = AB.Length();
	float radiiAB = rA + rB;
	return lengthAB <= radiiAB;
}

// MTV resolves B from A where t = translation, r = radius
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
		AB /= lengthAB;
		mtv = resolution * AB * (radiiAB - lengthAB);
	}
	return colliding;
}

// t = transform (translation & rotation), hh = half height, r = radius
inline bool CapsuleCapsule(const RigidTransform& tA, float hhA, float rA, const RigidTransform& tB, float hhB, float rB)
{
	using namespace DirectX::SimpleMath;
	Vector3 nearestA, nearestB;
	NearestSpheres(tA, hhA, rA, tB, hhB, rB, nearestA, nearestB);
	return SphereSphere(nearestA, rA, nearestB, rB);
}

// MTV resolves b from a where t = transform (translation & rotation), hh = half height, r = radius
inline bool CapsuleCapsule(const RigidTransform& tA, float hhA, float rA, const RigidTransform& tB, float hhB, float rB, DirectX::SimpleMath::Vector3& mtv)
{
	using namespace DirectX::SimpleMath;
	Vector3 nearestA, nearestB;
	NearestSpheres(tA, hhA, rA, tB, hhB, rB, nearestA, nearestB);
	return SphereSphere(nearestA, rA, nearestB, rB, mtv);
}

// Outputs the upper and lower bounds of a cylinder (relative to its forward vector) where t = transform (translation & rotation), hh = half height
inline void Bounds(const RigidTransform& t, float hh, DirectX::SimpleMath::Vector3& upper, DirectX::SimpleMath::Vector3& lower)
{
	using namespace DirectX::SimpleMath;
	const Vector3 front = t.Front();
	upper = t.Translation() + front * hh;
	lower = t.Translation() - front * hh;
}

// Returns the point along line ab closest to point p
inline DirectX::SimpleMath::Vector3 NearestPoint(const DirectX::SimpleMath::Vector3& a, const DirectX::SimpleMath::Vector3& b, const DirectX::SimpleMath::Vector3& p)
{
	using namespace DirectX::SimpleMath;
	Vector3 AB = b - a;
	float t = (p - a).Dot(AB) / AB.Dot(AB);
	return a + std::min(std::max(t, 0.0f), 1.0f) * AB;
}

inline void NearestSpheres(
	const RigidTransform& tA, float hhA, float rA,
	const RigidTransform& tB, float hhB, float rB,
	DirectX::SimpleMath::Vector3& nearestA,
	DirectX::SimpleMath::Vector3& nearestB)
{
	using namespace DirectX::SimpleMath;

	Vector3 aUpper;
	Vector3 aLower;
	Bounds(tA, hhA, aUpper, aLower);

	Vector3 bUpper;
	Vector3 bLower;
	Bounds(tB, hhB, bUpper, bLower);;

	Vector3 v0 = bLower - aLower;
	Vector3 v1 = bUpper - aLower;
	Vector3 v2 = bLower - aUpper;
	Vector3 v3 = bUpper - aUpper;

	float d0 = v0.Dot(v0);
	float d1 = v1.Dot(v1);
	float d2 = v2.Dot(v2);
	float d3 = v3.Dot(v3);

	Vector3 nearbyA = (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1) ? aUpper : aLower;
	nearestB = NearestPoint(bLower, bUpper, nearbyA);
	nearestA = NearestPoint(aLower, aUpper, nearestB);
}