#pragma once
#include "MathUtilities.h"

inline void CylinderBounds(const Vector3& position, const Vector3& direction, float halfHeight,
	Vector3& top, Vector3& bot)
{
	top = position + direction * halfHeight;
	bot = position - direction * halfHeight;
}

// Determines the two closest points along cylinders A and B
inline void NearestCylinderPoints(
	const Vector3& tA, const Vector3& tB,
	const Vector3& dA, const Vector3& dB,
	float hhA, float hhB,
	Vector3& nearestA, Vector3& nearestB)
{
	Vector3 aTop, aBot, bTop, bBot;
	CylinderBounds(tA, dA, hhA, aTop, aBot);
	CylinderBounds(tB, dB, hhB, bTop, bBot);

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
inline bool SphereSphere(
	const Vector3& tA, const Vector3& tB,
	float rA, float rB)
{
	Vector3 AB = tB - tA;
	float lengthAB = AB.Length();
	float radiiAB = rA + rB;
	return lengthAB <= radiiAB;
}

// MTV resolves B from A
inline bool SphereSphere(
	const Vector3& tA, const Vector3& tB,
	float rA, float rB, Vector3& mtv)
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

//*****************
// Capsule-Capsule
//*****************

// Boolean hit-test
inline bool CapsuleCapsule(
	const Vector3& tA, const Vector3& tB,
	const Vector3& dA, const Vector3& dB,
	float rA, float rB, float hhA, float hhB)
{
	Vector3 nearestA, nearestB;
	NearestCylinderPoints(tA, tB, dA, dB, hhA, hhB, nearestA, nearestB);
	return SphereSphere(nearestA, nearestB, rA, rB);
}

// MTV resolves b from a
inline bool CapsuleCapsule(
	const Vector3& tA, const Vector3& tB,
	const Vector3& dA, const Vector3& dB,
	float rA, float rB, float hhA, float hhB, Vector3& mtv)
{
	Vector3 nearestA, nearestB;
	NearestCylinderPoints(tA, tB, dA, dB, hhA, hhB, nearestA, nearestB);
	return SphereSphere(nearestA, nearestB, rA, rB, mtv);
}

//*****************
// Sphere-Capsule
//*****************

// Boolean hit-test
inline bool SphereCapsule(
	const Vector3& tA, const Vector3& tB, const Vector3& dB,
	float rA, float rB, float hhB)
{
	Vector3 top, bot;
	CylinderBounds(tB, dB, hhB, top, bot);
	return SphereSphere(tA, Project(bot, top, tA), rA, rB);
}

// MTV resolves b from a
inline bool SphereCapsule(
	const Vector3& tA, const Vector3& tB, const Vector3& dB,
	float rA, float rB, float hhB, Vector3& mtv)
{
	Vector3 top, bot;
	CylinderBounds(tB, dB, hhB, top, bot);
	return SphereSphere(tA, Project(bot, top, tA), rA, rB, mtv);
}

//*****************
// Miscellaneous
//*****************
inline bool InFoV(
	const Vector3& position, const Vector3& direction, const Vector3& target,
	float length, float degrees)
{
	if ((target - position).Length() > length) return false;
	Vector3 targetDirection = (target - position);
	targetDirection.Normalize();
	return targetDirection.Dot(direction) > cosf(DirectX::XM_RADIANS * degrees * 0.5f);
}
