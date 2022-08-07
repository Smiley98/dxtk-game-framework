#pragma once
#include "Tags.h"
#include "UnorderedVector.h"

#pragma region Types

struct Sphere
{
	Vector3	t;	// Translation
	float r;	// Radius
};

struct Capsule
{
	RigidTransform t;	// Transformation
	float hh;			// Half-height
	float r;			// Radius
};

struct ColliderInfo
{
	void* data = nullptr;	// Points to the object that owns this collider
	Tag tag = Tag::NONE;	// Used to cast data
};

#pragma endregion

#pragma region Math

// Outputs the top and bottom of a cylinder relative to its forward vector
inline void CylinderBounds(const Capsule& capsule, Vector3& top, Vector3& bot)
{
	const RigidTransform& t = capsule.t;
	const Vector3 front = t.Forward();
	top = t.Translation() + front * capsule.hh;
	bot = t.Translation() - front * capsule.hh;
}

// Returns the point along line ab closest to point p
inline Vector3 NearestPoint(const Vector3& a, const Vector3& b, const Vector3& p)
{
	Vector3 AB = b - a;
	float t = (p - a).Dot(AB) / AB.Dot(AB);
	return a + std::min(std::max(t, 0.0f), 1.0f) * AB;
}

inline void NearestSpheres(const Capsule a, const Capsule b, Vector3& nearestA, Vector3& nearestB)
{
	Vector3 aTop, aBot, bTop, bBot;
	CylinderBounds(a, aTop, aBot);
	CylinderBounds(b, bTop, bBot);

	Vector3 vectors[4] {
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

inline bool SphereSphere(const Sphere& a, const Sphere& b)
{
	Vector3 AB = b.t - a.t;
	float lengthAB = AB.Length();
	float radiiAB = a.r + b.r;
	return lengthAB <= radiiAB;
}

// MTV resolves B from A
inline bool SphereSphere(const Sphere& a, const Sphere& b, Vector3& mtv)
{
	Vector3 AB = b.t - a.t;
	float lengthAB = AB.Length();
	float radiiAB = a.r + b.r;
	bool colliding = lengthAB <= radiiAB;
	if (colliding)
	{
		constexpr float resolution = 1.0f + FLT_EPSILON * 16.0f;
		AB /= lengthAB;
		mtv = resolution * AB * (radiiAB - lengthAB);
	}
	return colliding;
}

inline bool SphereCapsule(const Sphere& a, const Capsule& b)
{
	Vector3 front = b.t.Forward();
	Vector3 cylinderTop = b.t.Translation() + front * b.hh;
	Vector3 cylinderBot = b.t.Translation() - front * b.hh;
	return SphereSphere(a, { NearestPoint(cylinderBot, cylinderTop, a.t), b.r });
}

// MTV resolves b from a
inline bool SphereCapsule(const Sphere& a, const Capsule& b, Vector3& mtv)
{
	Vector3 front = b.t.Forward();
	Vector3 cylinderTop = b.t.Translation() + front * b.hh;
	Vector3 cylinderBot = b.t.Translation() - front * b.hh;
	return SphereSphere(a, { NearestPoint(cylinderBot, cylinderTop, a.t), b.r }, mtv);
}

inline bool CapsuleCapsule(const Capsule& a, const Capsule& b)
{
	Vector3 nearestA, nearestB;
	NearestSpheres(a, b, nearestA, nearestB);
	return SphereSphere({ nearestA, a.r }, { nearestB, b.r });
}

// MTV resolves b from a
inline bool CapsuleCapsule(const Capsule& a, const Capsule& b, Vector3& mtv)
{
	Vector3 nearestA, nearestB;
	NearestSpheres(a, b, nearestA, nearestB);
	return SphereSphere({ nearestA, a.r }, { nearestB, b.r }, mtv);
}
#pragma endregion

#pragma region Colliders
struct SphereCollider;
struct CapsuleCollider;

struct SphereCollider
{
	Sphere g; // Geometry
	ColliderInfo info;

	inline bool IsColliding(const SphereCollider& collider) const;
	inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const;
	inline bool IsColliding(const CapsuleCollider& collider) const;
	inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const;
};

struct CapsuleCollider
{
	Capsule g; // Geometry
	ColliderInfo info;

	inline bool IsColliding(const CapsuleCollider& collider) const;
	inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const;
	inline bool IsColliding(const SphereCollider& collider) const;
	inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const;
};

struct StaticSphereId
{
	uint32_t n = 0;
};

struct DynamicSphereId
{
	uint32_t n = 0;
};

struct StaticCapsuleId
{
	uint32_t n = 0;
};

struct DynamicCapsuleId
{
	uint32_t n = 0;
};

// All that's needed to resolve collisions is mtv and references to the colliders owners.
// (Let the GameObject adjust its collider in response).
class Collision
{
public:
	enum Type
	{
		SPHERE_STATIC,
		SPHERE_DYNAMIC,
		CAPSULE_STATIC,
		CAPSULE_DYNAMIC
	};

	struct Id
	{
		uint32_t n = 0;
		Type type;
	};

	struct HitPair
	{
		Id a;
		Id b;
		Vector3 mtv;
	};

	inline Id AddStatic(Sphere&& geometry, ColliderInfo&& info);
	inline Id AddStatic(Capsule&& geometry, ColliderInfo&& info);
	inline Id AddDynamic(Sphere&& geometry, ColliderInfo&& info);
	inline Id AddDynamic(Capsule&& geometry, ColliderInfo&& info);

	inline void Remove(const Id& id);

	inline void Collide(std::vector<HitPair>& collisions);

private:
	UnorderedVector<SphereCollider> mStaticSpheres;
	UnorderedVector<SphereCollider> mDynamicSpheres;
	UnorderedVector<CapsuleCollider> mStaticCapsules;
	UnorderedVector<CapsuleCollider> mDynamicCapsules;
};

inline void Collision::Collide(std::vector<HitPair>& collisions)
{

}

inline bool SphereCollider::IsColliding(const SphereCollider& collider) const
{
	return SphereSphere(collider.g, g);
}

inline bool SphereCollider::IsColliding(const CapsuleCollider& collider) const
{
	return SphereCapsule(g, collider.g);
}

inline bool SphereCollider::IsColliding(const SphereCollider& collider, Vector3& mtv) const
{
	return SphereSphere(collider.g, g, mtv);
}

inline bool SphereCollider::IsColliding(const CapsuleCollider& collider, Vector3& mtv) const
{
	bool isColliding = SphereCapsule(g, collider.g, mtv);
	mtv = -mtv;
	return isColliding;
}

inline bool CapsuleCollider::IsColliding(const CapsuleCollider& collider) const
{
	return CapsuleCapsule(collider.g, g);
}

inline bool CapsuleCollider::IsColliding(const SphereCollider& collider) const
{
	return SphereCapsule(collider.g, g);
}

inline bool CapsuleCollider::IsColliding(const CapsuleCollider& collider, Vector3& mtv) const
{
	return CapsuleCapsule(collider.g, g, mtv);
}

inline bool CapsuleCollider::IsColliding(const SphereCollider& collider, Vector3& mtv) const
{
	return SphereCapsule(collider.g, g, mtv);
}

inline Collision::Id Collision::AddStatic(Sphere&& geometry, ColliderInfo&& info)
{
	return { mStaticSpheres.Add(geometry, info), Type::SPHERE_STATIC };
}

inline Collision::Id Collision::AddDynamic(Sphere&& geometry, ColliderInfo&& info)
{
	return { mDynamicSpheres.Add(geometry, info), Type::SPHERE_DYNAMIC };
}

inline Collision::Id Collision::AddStatic(Capsule&& geometry, ColliderInfo&& info)
{
	return { mStaticCapsules.Add(geometry, info), Type::CAPSULE_STATIC };
}

inline Collision::Id Collision::AddDynamic(Capsule&& geometry, ColliderInfo&& info)
{
	return { mDynamicCapsules.Add(geometry, info), Type::CAPSULE_DYNAMIC };
}

inline void Collision::Remove(const Id& id)
{
	switch (id.type)
	{
	case SPHERE_STATIC:
		mStaticSpheres.Remove(id.n);
		break;
	case SPHERE_DYNAMIC:
		mDynamicSpheres.Remove(id.n);
		break;
	case CAPSULE_STATIC:
		mStaticCapsules.Remove(id.n);
		break;
	case CAPSULE_DYNAMIC:
		mDynamicCapsules.Remove(id.n);
		break;
	}
}
#pragma endregion
