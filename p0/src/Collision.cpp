#include "pch.h"
#include "Collision.h"

namespace Collision
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	//std::vector<SphereCollider> Collider::sStaticSpheres;
	//std::vector<SphereCollider> Collider::sDynamicSpheres;
	//std::vector<CapsuleCollider> Collider::sStaticCapsules;
	//std::vector<CapsuleCollider> Collider::sDynamicCapsules;

	std::vector<SphereCollider> Collider::sSpheres(64);
	std::vector<CapsuleCollider> Collider::sCapsules(64);
	uint32_t Collider::sId;

	Collider::Collider(const Transform& transform, Tag tag, void* data) :
		mTransform(transform),
		mTag(tag),
		mData(data),
		mId(++sId)
	{
#if _DEBUG
		assert(tag != NONE);
#endif
	}

	SphereCollider& Collider::AddSphere(const Transform& transform, Tag tag, void* data)
	{
		sSpheres.push_back({ transform, tag, data });
		return sSpheres.back();
	}

	CapsuleCollider& Collider::AddCapsule(const Transform& transform, Tag tag, void* data)
	{
		sCapsules.push_back({ transform, tag, data });
		return sCapsules.back();
	}

	void Collider::Remove(const SphereCollider& collider)
	{
		for (auto i = sSpheres.begin(); i != sSpheres.end(); i++)
		{
			if (i->Id() == collider.Id())
				i = sSpheres.erase(i);
		}
	}

	void Collider::Remove(const CapsuleCollider& collider)
	{
		for (auto i = sCapsules.begin(); i != sCapsules.end(); i++)
		{
			if (i->Id() == collider.Id())
				i = sCapsules.erase(i);
		}
	}

	void Collider::Clear()
	{
		sSpheres.clear();
		sCapsules.clear();
		sId = 0;
	}

	std::vector<HitPair> Collider::Collisions()
	{
		return std::vector<HitPair>();
	}

	SphereCollider::SphereCollider(const Transform& transform, Tag tag, void* data) :
		Collider(transform, tag, data)
	{
	}

	bool SphereCollider::Collision(const SphereCollider& collider, Vector3& mtv)
	{
		const float ra = Radius();
		const float rb = collider.Radius();

		const Vector3 pa = Position();
		const Vector3 pb = collider.Position();

		return false;
	}

	bool SphereCollider::Collision(const CapsuleCollider& collider, Vector3& mtv)
	{
		const float rSphere = Radius();
		const float rCapsule = collider.Radius();

		const float hhCapsule = collider.HalfHeight();

		const Vector3 tSphere = Position(); 
		const Vector3 tCapsule = mTransform.Translation();
		const Vector3 oCapsule = mTransform.Front();

		return false;
	}

	CapsuleCollider::CapsuleCollider(const Transform& transform, Tag tag, void* data) :
		Collider(transform, tag, data)
	{
	}

	bool CapsuleCollider::Collision(const SphereCollider& collider, Vector3& mtv)
	{
		const float r = Radius();
		const float hh = HalfHeight();
		const Vector3 t = mTransform.Translation();
		const Vector3 o = mTransform.Front();

		return false;
	}

	bool CapsuleCollider::Collision(const CapsuleCollider& collider, Vector3& mtv)
	{
		const float ra = Radius();
		const float rb = collider.Radius();

		const float hha = HalfHeight();
		const float hhb = collider.HalfHeight();

		const Vector3 ta = mTransform.Translation();
		const Vector3 tb = mTransform.Translation();

		const Vector3 oa = mTransform.Front();
		const Vector3 ob = mTransform.Front();

		return false;
	}
}
