#pragma once
#include "Transform.h"

inline void Bounds(const RigidTransform& t, float hh, DirectX::SimpleMath::Vector3& top, DirectX::SimpleMath::Vector3& bot);
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

// Outputs the top and bottom of a cylinder (relative to its forward vector) where t = transform (translation & rotation), hh = half height
inline void Bounds(const RigidTransform& t, float hh, DirectX::SimpleMath::Vector3& top, DirectX::SimpleMath::Vector3& bot)
{
	using namespace DirectX::SimpleMath;
	const Vector3 front = t.Front();
	top = t.Translation() + front * hh;
	bot = t.Translation() - front * hh;
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
	Vector3 aTop, aBot, bTop, bBot;
	Bounds(tA, hhA, aTop, aBot);
	Bounds(tB, hhB, bTop, bBot);

	// vAB
	Vector3 vBotBot = bBot - aBot;
	Vector3 vBotTop = bTop - aBot;
	Vector3 vTopBot = bBot - aTop;
	Vector3 vTopTop = bTop - aTop;

	// dAB
	float dBotBot = vBotBot.Dot(vBotBot);
	float dBotTop = vBotTop.Dot(vBotTop);
	float dTopBot = vTopBot.Dot(vTopBot);
	float dTopTop = vTopTop.Dot(vTopTop);

	// If any of aTop are closer than anything b, use aTop otherwise use aBot
	nearestA = (dTopBot < dBotBot || dTopBot < dBotTop || dTopTop < dBotBot || dTopTop < dBotTop) ? aTop : aBot;

	nearestB = NearestPoint(bBot, bTop, nearestA);
	nearestA = NearestPoint(aBot, aTop, nearestB);
}
