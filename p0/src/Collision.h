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
	/*enum Type
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
	};*/

	struct Info
	{
		void* data = nullptr;				// Points to the object that owns this collider
		Tags::Tag tag = Tags::Tag::NONE;	// Used to cast data
	};

	struct HitPair
	{
		Info infoA;
		Info infoB;
		Vector3 mtv;
	};

	UnorderedVector<SphereCollider> mStaticSpheres;
	UnorderedVector<SphereCollider> mDynamicSpheres;
	UnorderedVector<CapsuleCollider> mStaticCapsules;
	UnorderedVector<CapsuleCollider> mDynamicCapsules;

	inline void Collide(std::vector<HitPair>& collisions);

//I can't be bothered to re-write UnorderedVector's Add/Get/Remove methods...
/*
	inline Id AddStaticSphere(Sphere&& geometry, Info&& info);
	inline Id AddDynamicSphere(Sphere&& geometry, Info&& info);
	inline Id AddStaticCapsule(Capsule&& geometry, Info&& info);
	inline Id AddDynamicCapsule(Capsule&& geometry, Info&& info);

	inline SphereCollider& GetStaticSphere(const Id& id);
	inline SphereCollider& GetDynamicSphere(const Id& id);
	inline CapsuleCollider& GetStaticCapsule(const Id& id);
	inline CapsuleCollider& GetDynamicCapsule(const Id& id);

	inline void RemoveStaticSphere(const Id& id);
	inline void RemoveDynamicSphere(const Id& id);
	inline void RemoveStaticCapsule(const Id& id);
	inline void RemoveDynamicCapsule(const Id& id);

private:
	UnorderedVector<SphereCollider> mStaticSpheres;
	UnorderedVector<SphereCollider> mDynamicSpheres;
	UnorderedVector<CapsuleCollider> mStaticCapsules;
	UnorderedVector<CapsuleCollider> mDynamicCapsules;
*/
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
	
	// Static spheres vs dynamic spheres & dynamic capsules
	for (int i = 0; i < staticSpheres.size() - 1; i++)
	{
		for (int j = 0; j < dynamicSpheres.size(); j++)
		{
			Vector3 mtv;
			SphereCollider& a = staticSpheres[i];
			SphereCollider& b = dynamicSpheres[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}

		for (int j = 0; j < dynamicCapsules.size(); j++)
		{
			Vector3 mtv;
			SphereCollider& a = staticSpheres[i];
			CapsuleCollider& b = dynamicCapsules[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}

	// Static capsules vs dynamic spheres & dynamic capsules
	for (int i = 0; i < staticCapsules.size() - 1; i++)
	{
		for (int j = 0; j < dynamicSpheres.size(); j++)
		{
			Vector3 mtv;
			CapsuleCollider& a = staticCapsules[i];
			SphereCollider& b = dynamicSpheres[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}

		for (int j = 0; j < dynamicCapsules.size(); j++)
		{
			Vector3 mtv;
			CapsuleCollider& a = staticCapsules[i];
			CapsuleCollider& b = dynamicCapsules[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}

	// Dynamic spheres vs dynamic spheres & dynamic capsules
	for (int i = 0; i < dynamicSpheres.size(); i++)
	{
		for (int j = 0; j < dynamicCapsules.size(); j++)
		{
			Vector3 mtv;
			SphereCollider& a = dynamicSpheres[i];
			CapsuleCollider& b = dynamicCapsules[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}

		for (int j = 0; j < dynamicSpheres.size(); j++)
		{
			if (i == j) continue;
			Vector3 mtv;
			SphereCollider& a = dynamicSpheres[i];
			SphereCollider& b = dynamicSpheres[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}

	// Dynamic capsules vs dynamic capsules
	for (int i = 0; i < dynamicCapsules.size(); i++)
	{
		for (int j = 0; j < dynamicCapsules.size(); j++)
		{
			if (i == j) continue;
			Vector3 mtv;
			CapsuleCollider& a = dynamicCapsules[i];
			CapsuleCollider& b = dynamicCapsules[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}
}

/*
inline Collision::Id Collision::AddStaticSphere(Sphere&& geometry, Info&& info)
{
	Id id;
	id.type = SPHERE_STATIC;
	id.n = mStaticSpheres.Add(geometry, info);
	return id;
}

inline Collision::Id Collision::AddDynamicSphere(Sphere&& geometry, Info&& info)
{
	Id id;
	id.type = SPHERE_DYNAMIC;
	id.n = mDynamicSpheres.Add(geometry, info);
	return id;
}

inline Collision::Id Collision::AddStaticCapsule(Capsule&& geometry, Info&& info)
{
	Id id;
	id.type = CAPSULE_STATIC;
	id.n = mStaticCapsules.Add(geometry, info);
	return id;
}

inline Collision::Id Collision::AddDynamicCapsule(Capsule&& geometry, Info&& info)
{
	Id id;
	id.type = CAPSULE_DYNAMIC;
	id.n = mDynamicCapsules.Add(geometry, info);
	return id;
}

inline SphereCollider& Collision::GetStaticSphere(const Id& id)
{
	assert(id.type == SPHERE_STATIC);
	return mStaticSpheres.Get(id.n);
}

inline SphereCollider& Collision::GetDynamicSphere(const Id& id)
{
	assert(id.type == SPHERE_DYNAMIC);
	return mDynamicSpheres.Get(id.n);
}

inline CapsuleCollider& Collision::GetStaticCapsule(const Id& id)
{
	assert(id.type == CAPSULE_STATIC);
	return mStaticCapsules.Get(id.n);
}

inline CapsuleCollider& Collision::GetDynamicCapsule(const Id& id)
{
	assert(id.type == CAPSULE_DYNAMIC);
	return mDynamicCapsules.Get(id.n);
}

inline void Collision::RemoveStaticSphere(const Id& id)
{
	assert(id.type == SPHERE_STATIC);
	mStaticSpheres.Remove(id.n);
}

inline void Collision::RemoveDynamicSphere(const Id& id)
{
	assert(id.type == SPHERE_DYNAMIC);
	mDynamicSpheres.Remove(id.n);
}

inline void Collision::RemoveStaticCapsule(const Id& id)
{
	assert(id.type == CAPSULE_STATIC);
	mDynamicCapsules.Remove(id.n);
}

inline void Collision::RemoveDynamicCapsule(const Id& id)
{
	assert(id.type == CAPSULE_DYNAMIC);
	mDynamicSpheres.Remove(id.n);
}
*/

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
