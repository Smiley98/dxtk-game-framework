#pragma once
#include "Transform.h"

inline void Extents(const RigidTransform& t, float hh, float r, DirectX::SimpleMath::Vector3& topLeft, DirectX::SimpleMath::Vector3& botRight);
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

// Calculates the top left and bottom right of a capsule where t = transform (translation & rotation), hh = half height, r = radius
inline void Extents(const RigidTransform& t, float hh, float r, DirectX::SimpleMath::Vector3& topLeft, DirectX::SimpleMath::Vector3& botRight)
{
	using namespace DirectX::SimpleMath;
	float length = hh + r;
	Vector3 front = t.Front();
	Vector3 tip = t.Translation() + front * length;
	Vector3 base = t.Translation() - front * length;
	Vector3 norm = tip - base;
	norm.Normalize();
	Vector3 ortho = norm * r;
	topLeft = tip - ortho;
	botRight = base + ortho;
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

	Vector3 aTopLeft;
	Vector3 aBotRight;
	Extents(tA, hhA, rA, aTopLeft, aBotRight);

	Vector3 bTopLeft;
	Vector3 bBotRight;
	Extents(tB, hhB, rB, bTopLeft, bBotRight);;

	Vector3 v0 = bBotRight - aBotRight;
	Vector3 v1 = bTopLeft - aBotRight;
	Vector3 v2 = bBotRight - aTopLeft;
	Vector3 v3 = bTopLeft - aTopLeft;

	float d0 = v0.Dot(v0);
	float d1 = v1.Dot(v1);
	float d2 = v2.Dot(v2);
	float d3 = v3.Dot(v3);

	Vector3 nearbyA = (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1) ? aTopLeft : aBotRight;
	nearestB = NearestPoint(bBotRight, bTopLeft, nearbyA);
	nearestA = NearestPoint(aBotRight, aTopLeft, nearestB);
}