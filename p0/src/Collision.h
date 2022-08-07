#pragma once
#include "CollisionMath.h"
#include "UnorderedVector.h"
#include "Tags.h"

struct SphereCollider;
struct CapsuleCollider;

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

	struct Info
	{
		void* data = nullptr;	// Points to the object that owns this collider
		Tag tag = Tag::NONE;	// Used to cast data
	};

	struct HitPair
	{
		Info infoA;
		Info infoB;
		Vector3 mtv;
	};

	inline Id AddStatic(Sphere&& geometry, Info&& info);
	inline Id AddStatic(Capsule&& geometry, Info&& info);
	inline Id AddDynamic(Sphere&& geometry, Info&& info);
	inline Id AddDynamic(Capsule&& geometry, Info&& info);

	inline void Remove(const Id& id);

	inline void Collide(std::vector<HitPair>& collisions);

private:
	UnorderedVector<SphereCollider> mStaticSpheres;
	UnorderedVector<SphereCollider> mDynamicSpheres;
	UnorderedVector<CapsuleCollider> mStaticCapsules;
	UnorderedVector<CapsuleCollider> mDynamicCapsules;
};

struct SphereCollider
{
	Sphere g;
	Collision::Info info;

	inline bool IsColliding(const SphereCollider& collider) const;
	inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const;
	inline bool IsColliding(const CapsuleCollider& collider) const;
	inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const;
};

struct CapsuleCollider
{
	Capsule g;
	Collision::Info info;

	inline bool IsColliding(const CapsuleCollider& collider) const;
	inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const;
	inline bool IsColliding(const SphereCollider& collider) const;
	inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const;
};

inline void Collision::Collide(std::vector<HitPair>& collisions)
{
	std::vector<SphereCollider>& staticSpheres = mStaticSpheres.Objects();
	std::vector<SphereCollider>& dynamicSpheres = mDynamicSpheres.Objects();
	std::vector<CapsuleCollider>& staticCapsules = mStaticCapsules.Objects();
	std::vector<CapsuleCollider>& dynamicCapsules = mDynamicCapsules.Objects();
	
	for (int i = 0; i < staticSpheres.size() - 1; i++)
	{
		for (int j = i; j < staticSpheres.size(); j++)
		{
			Vector3 mtv;
			SphereCollider& a = staticSpheres[i];
			SphereCollider& b = staticSpheres[j];
			if (SphereSphere(a.g, b.g, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}
	
	/*
	for (int i = 0; i < capsules.size() - 1; i++)
	{
		for (int j = i; j < capsules.size(); j++)
		{
			Vector3 mtv;
			CapsuleCollider& a = capsules[i];
			CapsuleCollider& b = capsules[j];
			if (CapsuleCapsule(a.g, b.g, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}
	
	for (int i = 0; i < spheres.size(); i++)
	{
		for (int j = 0; j < capsules.size(); j++)
		{
			Vector3 mtv;
			SphereCollider& a = spheres[i];
			CapsuleCollider& b = capsules[j];
			if (SphereCapsule(a.g, b.g, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}*/
}

inline Collision::Id Collision::AddStatic(Sphere&& geometry, Info&& info)
{
	Id id;
	id.type = SPHERE_STATIC;
	id.n = mStaticSpheres.Add(geometry, info);
	return id;
}

inline Collision::Id Collision::AddDynamic(Sphere&& geometry, Info&& info)
{
	Id id;
	id.type = SPHERE_DYNAMIC;
	id.n = mDynamicSpheres.Add(geometry, info);
	return id;
}

inline Collision::Id Collision::AddStatic(Capsule&& geometry, Info&& info)
{
	Id id;
	id.type = CAPSULE_STATIC;
	id.n = mStaticCapsules.Add(geometry, info);
	return id;
}

inline Collision::Id Collision::AddDynamic(Capsule&& geometry, Info&& info)
{
	Id id;
	id.type = CAPSULE_DYNAMIC;
	id.n = mDynamicCapsules.Add(geometry, info);
	return id;
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
