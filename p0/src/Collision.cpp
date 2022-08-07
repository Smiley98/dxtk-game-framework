#include "pch.h"
#include "Collision.h"


SphereCollider& Collision::Add(const Sphere& geometry, ColliderInfo info)
{
	mSpheres.push_back({ info, geometry });
	SphereCollider& collider = mSpheres.back();
	mSphereMap[&collider] = mSpheres.size() - 1;
	return collider;
}

CapsuleCollider& Collision::Add(const Capsule& geometry, ColliderInfo info)
{
	mCapsules.push_back({ info, geometry });
	CapsuleCollider& collider = mCapsules.back();
	mCapsuleMap[&collider] = mCapsules.size() - 1;
	return collider;
}

void Collision::Remove(const SphereCollider& collider)
{
	size_t back = mSpheres.size() - 1;
}

void Collision::Remove(const CapsuleCollider& collider)
{
	size_t back = mCapsules.size() - 1;
}

std::vector<HitPair> Collision::Collide()
{
	std::vector<HitPair> collisions;

	for (int i = 0; i < mSpheres.size() - 1; i++)
	{
		for (int j = i; j < mSpheres.size(); j++)
		{
			Vector3 mtv;
			SphereCollider& a = mSpheres[i];
			SphereCollider& b = mSpheres[j];
			if (SphereSphere(a.g, b.g, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}

	for (int i = 0; i < mCapsules.size() - 1; i++)
	{
		for (int j = i; j < mCapsules.size(); j++)
		{
			Vector3 mtv;
			CapsuleCollider& a = mCapsules[i];
			CapsuleCollider& b = mCapsules[j];
			if (CapsuleCapsule(a.g, b.g, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}

	for (int i = 0; i < mSpheres.size(); i++)
	{
		for (int j = 0; j < mCapsules.size(); j++)
		{
			Vector3 mtv;
			SphereCollider& a = mSpheres[i];
			CapsuleCollider& b = mCapsules[j];
			if (SphereCapsule(a.g, b.g, mtv))
			{
				collisions.push_back({ a.info, b.info, mtv });
			}
		}
	}

	return collisions;
}
