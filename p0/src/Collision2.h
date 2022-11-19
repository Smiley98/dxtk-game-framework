#pragma once
#include "CollisionMath2.h"
#include "Entity.h"
#include <array>
#include <vector>

struct Components;

// This file is an excellent example of everything wrong with C++ and object-oriented programming.
namespace Collision2
{
	class SphereCollider;
	class CapsuleCollider;

	struct HitPair
	{
		std::array<Entity, 2> hits { INVALID_ENTITY, INVALID_ENTITY };
		Vector3 mtv;
	};

	void Collide(
		const std::vector<Entity>& entities,
		const Components& components,
		std::vector<HitPair>& collisions);

	class Collider
	{
	public:
		Collider(const Entity e, const Transform3& t) : entity(e), transform(t) {}

	protected:
		virtual bool IsColliding(const SphereCollider& collider) const = 0;
		virtual bool IsColliding(const CapsuleCollider& collider) const = 0;
		virtual bool IsColliding(const SphereCollider& collider, Vector3& mtv) const = 0;
		virtual bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const = 0;

		const Entity entity = INVALID_ENTITY;
		const Transform3& transform;

	private:
		Collider() = default;
	};

	class SphereCollider :
		public Collider
	{
	public:
		SphereCollider(const Entity& e, const Transform3& t, const Sphere& g) : Collider(e, t), geometry(g) {}
		friend CapsuleCollider;
		friend void Collide(
			const std::vector<Entity>& entities,
			const Components& components,
			std::vector<HitPair>& collisions);

	protected:
		const Sphere& geometry;

		inline bool IsColliding(const SphereCollider& collider) const final;
		inline bool IsColliding(const CapsuleCollider& collider) const final;
		inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const final;
		inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const final;

	private:
		SphereCollider() = default;
	};

	class CapsuleCollider :
		public Collider
	{
	public:
		CapsuleCollider(const Entity& e, const Transform3& t, const Capsule& g) : Collider(e, t), geometry(g) {}
		friend SphereCollider;
		friend void Collide(
			const std::vector<Entity>& entities,
			const Components& components,
			std::vector<HitPair>& collisions);

	protected:
		const Capsule& geometry;

		inline bool IsColliding(const CapsuleCollider& collider) const final;
		inline bool IsColliding(const SphereCollider& collider) const final;
		inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const final;
		inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const final;

	private:
		CapsuleCollider() = default;
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
}
