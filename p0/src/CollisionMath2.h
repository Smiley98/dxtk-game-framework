#pragma once
#include "Transform3.h"
#include "Geometry.h"
#undef min
#undef max

// Outputs the top and bottom of a cylinder relative to its forward vector
inline void CylinderBounds(const Transform3& transform, float halfHeight, Vector3& top, Vector3& bot)
{
	Vector3 forward = transform.Forward();
	top = transform.Translation() + forward * halfHeight;
	bot = transform.Translation() - forward * halfHeight;
}

// Projects point P along line AB
inline Vector3 Project(const Vector3& a, const Vector3& b, const Vector3& p)
{
	Vector3 AB = b - a;
	float t = (p - a).Dot(AB) / AB.Dot(AB);
	return a + std::min(std::max(t, 0.0f), 1.0f) * AB;
}

// Determines the two closest points along cylinders A and B
inline void NearestCylinderPoints(
	const Transform3& tA, const Transform3& tB, float hhA, float hhB,
	Vector3& nearestA, Vector3& nearestB)
{
	Vector3 aTop, aBot, bTop, bBot;
	CylinderBounds(tA, hhA, aTop, aBot);
	CylinderBounds(tB, hhB, bTop, bBot);

	Vector3 vectors[4]{
		bTop - aTop,	// A-top to B-top
		bBot - aTop,	// A-top to B-bot
		bTop - aBot,	// A-bot to B-top
		bBot - aBot		// A-bot to B-bot
	};

	size_t min = 0;
	float length = vectors[min].LengthSquared();
	for (size_t i = 1; i < 4; i++)
	{
		float temp = vectors[i].LengthSquared();
		if (temp < length)
		{
			length = temp;
			min = i;
		}
	}

	nearestA = min < 2 ? aTop : aBot;
	nearestB = Project(bBot, bTop, nearestA);
	nearestA = Project(aBot, aTop, nearestB);
}

//*****************
// Sphere-Sphere
//*****************

// Boolean hit-test
inline bool SphereSphere(const Vector3& tA, const Vector3& tB, float rA, float rB)
{
	Vector3 AB = tB - tA;
	float lengthAB = AB.Length();
	float radiiAB = rA + rB;
	return lengthAB <= radiiAB;
}

// MTV resolves B from A
inline bool SphereSphere(const Vector3& tA, const Vector3& tB, float rA, float rB, Vector3& mtv)
{
	Vector3 AB = tB - tA;
	float lengthAB = AB.Length();
	float radiiAB = rA + rB;
	bool colliding = lengthAB <= radiiAB;
	if (colliding)
	{
		constexpr float resolution = 1.0f + FLT_EPSILON * 16.0f;
		AB.Normalize();
		mtv = resolution * AB * (radiiAB - lengthAB);
	}
	return colliding;
}

// Boolean wrapper
inline bool SphereSphere(const Transform3& tA, const Transform3& tB, const Sphere& gA, const Sphere& gB)
{
	return SphereSphere(tA.Translation(), tB.Translation(), gA.r, gB.r);
}

// MTV wrapper
inline bool SphereSphere(const Transform3& tA, const Transform3& tB, const Sphere& gA, const Sphere& gB, Vector3& mtv)
{
	return SphereSphere(tA.Translation(), tB.Translation(), gA.r, gB.r, mtv);
}

//*****************
// Sphere-Capsule
//*****************

// Boolean hit-test
inline bool SphereCapsule(const Vector3& tA, const Transform3& tB, float rA, float rB, float hhB)
{
	Vector3 top, bot;
	CylinderBounds(tB, hhB, top, bot);
	return SphereSphere(tA, Project(bot, top, tA), rA, rB);
}

// MTV resolves b from a
inline bool SphereCapsule(const Vector3& tA, const Transform3& tB, float rA, float rB, float hhB, Vector3& mtv)
{
	Vector3 top, bot;
	CylinderBounds(tB, hhB, top, bot);
	return SphereSphere(tA, Project(bot, top, tA), rA, rB, mtv);
}

// Boolean wrapper
inline bool SphereCapsule(const Transform3& tA, const Transform3& tB, const Sphere& gA, const Capsule& gB)
{
	return SphereCapsule(tA.Translation(), tB, gA.r, gB.r, gB.hh);
}

// MTV wrapper
inline bool SphereCapsule(const Transform3& tA, const Transform3& tB, const Sphere& gA, const Capsule& gB, Vector3& mtv)
{
	return SphereCapsule(tA.Translation(), tB, gA.r, gB.r, gB.hh, mtv);
}

//*****************
// Capsule-Capsule
//*****************

// Boolean hit-test
inline bool CapsuleCapsule(const Transform3& tA, const Transform3& tB, float rA, float rB, float hhA, float hhB)
{
	Vector3 nearestA, nearestB;
	NearestCylinderPoints(tA, tB, hhA, hhB, nearestA, nearestB);
	return SphereSphere(nearestA, nearestB, rA, rB);
}

// MTV resolves b from a
inline bool CapsuleCapsule(const Transform3& tA, const Transform3& tB, float rA, float rB, float hhA, float hhB, Vector3& mtv)
{
	Vector3 nearestA, nearestB;
	NearestCylinderPoints(tA, tB, hhA, hhB, nearestA, nearestB);
	return SphereSphere(nearestA, nearestB, rA, rB, mtv);
}

// Boolean wrapper
inline bool CapsuleCapsule(const Transform3& tA, const Transform3& tB, const Capsule& gA, const Capsule& gB)
{
	return CapsuleCapsule(tA, tB, gA.r, gB.r, gA.hh, gB.hh);
}

// MTV wrapper
inline bool CapsuleCapsule(const Transform3& tA, const Transform3& tB, const Capsule& gA, const Capsule& gB, Vector3& mtv)
{
	return CapsuleCapsule(tA, tB, gA.r, gB.r, gA.hh, gB.hh, mtv);
}

//*****************
// Miscellaneous
//*****************
inline bool InRange(const Transform3& viewer, const Vector3& target, float length, float fov /*(degrees)*/)
{
	if ((target - viewer.Translation()).Length() > length)
		return false;

	Vector3 viewerDirection = viewer.Forward();
	Vector3 targetDirection = (target - viewer.Translation());
	targetDirection.Normalize();

	return targetDirection.Dot(viewerDirection) > cosf(DirectX::XM_RADIANS * fov * 0.5f);
}
