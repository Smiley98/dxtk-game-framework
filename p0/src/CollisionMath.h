#pragma once
#include "Transform.h"
#include <utility>
#undef min
#undef max

namespace Collision
{
	// Outputs the top and bottom of a cylinder relative to its forward vector
	inline void CylinderBounds(const RigidTransform& transform, float halfHeight, Vector3& top, Vector3& bot)
	{
		Vector3 forward = transform.Forward();
		top = transform.Translation() + forward * halfHeight;
		bot = transform.Translation() - forward * halfHeight;
	}

	// Returns the point along line ab closest to point p
	inline Vector3 NearestPoint(const Vector3& a, const Vector3& b, const Vector3& p)
	{
		Vector3 AB = b - a;
		float t = (p - a).Dot(AB) / AB.Dot(AB);
		return a + std::min(std::max(t, 0.0f), 1.0f) * AB;
	}

	// Outputs nearest spheres on capsules A and B
	inline void NearestSpheres(const RigidTransform& tA, const RigidTransform& tB, float hhA, float hhB, float rA, float rB,
		Vector3& nearestA, Vector3& nearestB)
	{
		Vector3 aTop, aBot, bTop, bBot;
		CylinderBounds(tA, hhA, aTop, aBot);
		CylinderBounds(tB, hhB, bTop, bBot);

		Vector3 vectors[4]{
			bTop - aTop,
			bBot - aTop,
			bTop - aBot,
			bBot - aBot
		};

		uint32_t min = 0;
		float length = vectors[min].LengthSquared();
		for (int i = 1; i < 4; i++)
		{
			float temp = vectors[i].LengthSquared();
			if (temp < length)
			{
				length = temp;
				min = i;
			}
		}

		nearestA = min < 2 ? aTop : aBot;
		nearestB = NearestPoint(bBot, bTop, nearestA);
		nearestA = NearestPoint(aBot, aTop, nearestB);
	}

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

	inline bool SphereCapsule(const Vector3& tA, float rA, const RigidTransform& tB, float hhB, float rB)
	{
		Vector3 top, bot;
		CylinderBounds(tB, hhB, top, bot);
		return SphereSphere(tA, NearestPoint(bot, top, tA), rA, rB);
	}

	// MTV resolves b from a
	inline bool SphereCapsule(const Vector3& tA, float rA, const RigidTransform& tB, float hhB, float rB, Vector3& mtv)
	{
		Vector3 top, bot;
		CylinderBounds(tB, hhB, top, bot);
		return SphereSphere(tA, NearestPoint(bot, top, tA), rA, rB, mtv);
	}

	inline bool CapsuleCapsule(const RigidTransform& tA, const RigidTransform& tB, float hhA, float hhB, float rA, float rB)
	{
		Vector3 nearestA, nearestB;
		NearestSpheres(tA, tB, hhA, hhB, rA, rB, nearestA, nearestB);
		return SphereSphere(nearestA, nearestB, rA, rB);
	}

	// MTV resolves b from a
	inline bool CapsuleCapsule(const RigidTransform& tA, const RigidTransform& tB, float hhA, float hhB, float rA, float rB, Vector3& mtv)
	{
		Vector3 nearestA, nearestB;
		NearestSpheres(tA, tB, hhA, hhB, rA, rB, nearestA, nearestB);
		return SphereSphere(nearestA, nearestB, rA, rB, mtv);
	}

	inline bool InRange(const TransformBase& viewer, const Vector3& target, float length, float fov /*(degrees)*/)
	{
		if ((target - viewer.Translation()).Length() > length)
			return false;

		Vector3 viewerDirection = viewer.Forward();
		Vector3 targetDirection = (target - viewer.Translation());
		targetDirection.Normalize();

		return targetDirection.Dot(viewerDirection) > cosf(DirectX::XM_RADIANS * fov * 0.5f);
	}

	//inline bool SphereTriangle(const Vector3& tA, float rA, const Vector3& p0, const Vector3& p1, const Vector3& p2)
	//{
	//
	//}

	// Creating from top & bot is less efficient due to the additional rotation calculations.
	// Moreover, the math maps nicely to RigidTransform and hh and r are models y and x.
	//float hh = (top - bot).Length() * 0.5f - radius);
	//Vector3 translation = (top + bot) * 0.5f;
	//Vector3 orientation;
	//(top - bot).Normalize(orientation);
	//Orientate(orientation);
}
