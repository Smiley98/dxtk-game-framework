#include "pch.h"
#include "Collision.h"

	//std::vector<SphereCollider>& spheres = mSpheres.Objects();
	//std::vector<CapsuleCollider>& capsules = mCapsules.Objects();
	//
	//for (int i = 0; i < spheres.size() - 1; i++)
	//{
	//	for (int j = i; j < spheres.size(); j++)
	//	{
	//		Vector3 mtv;
	//		SphereCollider& a = spheres[i];
	//		SphereCollider& b = spheres[j];
	//		if (SphereSphere(a.g, b.g, mtv))
	//		{
	//			collisions.push_back({ a.info, b.info, mtv });
	//		}
	//	}
	//}
	//
	//for (int i = 0; i < capsules.size() - 1; i++)
	//{
	//	for (int j = i; j < capsules.size(); j++)
	//	{
	//		Vector3 mtv;
	//		CapsuleCollider& a = capsules[i];
	//		CapsuleCollider& b = capsules[j];
	//		if (CapsuleCapsule(a.g, b.g, mtv))
	//		{
	//			collisions.push_back({ a.info, b.info, mtv });
	//		}
	//	}
	//}
	//
	//for (int i = 0; i < spheres.size(); i++)
	//{
	//	for (int j = 0; j < capsules.size(); j++)
	//	{
	//		Vector3 mtv;
	//		SphereCollider& a = spheres[i];
	//		CapsuleCollider& b = capsules[j];
	//		if (SphereCapsule(a.g, b.g, mtv))
	//		{
	//			collisions.push_back({ a.info, b.info, mtv });
	//		}
	//	}
	//}
