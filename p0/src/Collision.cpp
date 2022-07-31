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

	Collider::Collider(Tag tag, void* data) :
		mId(++sId),
		mTag(tag),
		mData(data)
	{
#if _DEBUG
		assert(tag != NONE);
#endif
	}

	uint32_t Collider::Id() const
	{
		return mId;
	}

	SphereCollider& Collider::Add(const DirectX::SimpleMath::Vector3& translation, float radius,
		Tag tag, void* data)
	{
#if _DEBUG
		assert(tag != NONE);
#endif
		sSpheres.push_back({ translation, radius, tag, data });
		return sSpheres.back();
	}

	CapsuleCollider& Collider::Add(const DirectX::SimpleMath::Vector3& translation, const DirectX::SimpleMath::Vector3& orientation, float halfHeight, float radius,
		Tag tag, void* data)
	{
#if _DEBUG
		assert(tag != NONE);
#endif
		sCapsules.push_back({ translation, orientation, halfHeight, radius, tag, data });
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

	SphereCollider::SphereCollider(const DirectX::SimpleMath::Vector3& translation,
		float radius, Tag tag, void* data) :
		Collider(tag, data),
		mTranslation(translation),
		mRadius(radius)
	{
	}

	bool SphereCollider::Collision(const SphereCollider& collider, Vector3& mtv)
	{
		return false;
	}

	bool SphereCollider::Collision(const CapsuleCollider& collider, Vector3& mtv)
	{
		return false;
	}

	void SphereCollider::Update(const Transform& transform)
	{

	}

	CapsuleCollider::CapsuleCollider(const DirectX::SimpleMath::Vector3& translation, const DirectX::SimpleMath::Vector3& orientation,
		float halfHeight, float radius, Tag tag, void* data) :
		Collider(tag, data),
		mTranslation(translation),
		mOrientation(orientation),
		mHalfHeight(halfHeight),
		mRadius(radius)
	{
	}

	bool CapsuleCollider::Collision(const SphereCollider& collider, Vector3& mtv)
	{
		return false;
	}

	bool CapsuleCollider::Collision(const CapsuleCollider& collider, Vector3& mtv)
	{
		return false;
	}

	void CapsuleCollider::Update(const Transform& transform)
	{

	}
}
