#pragma once
#include "CollisionMath.h"
#include "Entity.h"
#include <array>
#include <vector>

struct Components;

// This file is an excellent example of everything wrong with C++ and object-oriented programming.
struct HitPair
{
	std::array<Entity, 2> hits { INVALID_ENTITY, INVALID_ENTITY };
	Vector3 mtv;
};

void Collide(const Components& components, std::vector<HitPair>& collisions);

class SphereCollider;
class CapsuleCollider;
class Collider
{
public:
	const Entity entity = INVALID_ENTITY;
	const Transform3& transform;

protected:
	Collider() = default;
	Collider(const Entity e, const Transform3& t) : entity(e), transform(t) {}

	virtual bool IsColliding(const SphereCollider& collider) const = 0;
	virtual bool IsColliding(const CapsuleCollider& collider) const = 0;
	virtual bool IsColliding(const SphereCollider& collider, Vector3& mtv) const = 0;
	virtual bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const = 0;
};

class SphereCollider :
	public Collider
{
public:
	const Sphere& geometry;

protected:
	inline bool IsColliding(const SphereCollider& collider) const final;
	inline bool IsColliding(const CapsuleCollider& collider) const final;
	inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const final;
	inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const final;

private:
	SphereCollider() = default;
	SphereCollider(const Entity& e, const Transform3& t, const Sphere& g) : Collider(e, t), geometry(g) {}

	friend void Collide(const Components& components, std::vector<HitPair>& collisions);
};

class CapsuleCollider :
	public Collider
{
public:
	const Capsule& geometry;

protected:
	inline bool IsColliding(const CapsuleCollider& collider) const final;
	inline bool IsColliding(const SphereCollider& collider) const final;
	inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const final;
	inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const final;

private:
	CapsuleCollider(const Entity& e, const Transform3& t, const Capsule& g) : Collider(e, t), geometry(g) {}
	CapsuleCollider() = default;

	friend void Collide(const Components& components, std::vector<HitPair>& collisions);
};

inline bool SphereCollider::IsColliding(const SphereCollider& collider) const
{
	return SphereSphere(collider.transform, transform, collider.geometry, geometry);
}

inline bool SphereCollider::IsColliding(const CapsuleCollider& collider) const
{
	return SphereCapsule(transform, collider.transform, geometry, collider.geometry);
}

inline bool SphereCollider::IsColliding(const SphereCollider& collider, Vector3& mtv) const
{
	return SphereSphere(collider.transform, transform, collider.geometry, geometry, mtv);
}

inline bool SphereCollider::IsColliding(const CapsuleCollider& collider, Vector3& mtv) const
{
	bool isColliding = SphereCapsule(transform, collider.transform, geometry, collider.geometry, mtv);
	mtv = -mtv;
	return isColliding;
}

inline bool CapsuleCollider::IsColliding(const CapsuleCollider& collider) const
{
	return CapsuleCapsule(collider.transform, transform, collider.geometry, geometry);
}

inline bool CapsuleCollider::IsColliding(const SphereCollider& collider) const
{
	return SphereCapsule(collider.transform, transform, collider.geometry, geometry);
}

inline bool CapsuleCollider::IsColliding(const CapsuleCollider& collider, Vector3& mtv) const
{
	return CapsuleCapsule(collider.transform, transform, collider.geometry, geometry, mtv);
}

inline bool CapsuleCollider::IsColliding(const SphereCollider& collider, Vector3& mtv) const
{
	return SphereCapsule(collider.transform, transform, collider.geometry, geometry, mtv);
}