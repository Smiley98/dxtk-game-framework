#include "pch.h"
#include "Collision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//std::vector<SphereCollider> Collider::sStaticSpheres;
//std::vector<SphereCollider> Collider::sDynamicSpheres;
//std::vector<CapsuleCollider> Collider::sStaticCapsules;
//std::vector<CapsuleCollider> Collider::sDynamicCapsules;

std::vector<SphereCollider> Collision::sSpheres(64);
std::vector<CapsuleCollider> Collision::sCapsules(64);
uint32_t Collision::sId;

SphereCollider& Collision::Add(const DirectX::SimpleMath::Vector3& translation, float radius,
	Tag tag, void* data)
{
#if _DEBUG
	assert(tag != NONE);
#endif
	sSpheres.push_back({ translation, radius, tag, data, ++sId });
	return sSpheres.back();
}

CapsuleCollider& Collision::Add(const RigidTransform& transform, float halfHeight, float radius,
	Tag tag, void* data)
{
#if _DEBUG
	assert(tag != NONE);
#endif
	sCapsules.push_back({ transform, halfHeight, radius, tag, data, ++sId });
	return sCapsules.back();
}

void Collision::Remove(const SphereCollider& collider)
{
	for (auto i = sSpheres.begin(); i != sSpheres.end(); i++)
	{
		if (i->info.id == collider.info.id)
			i = sSpheres.erase(i);
	}
}

void Collision::Remove(const CapsuleCollider& collider)
{
	for (auto i = sCapsules.begin(); i != sCapsules.end(); i++)
	{
		if (i->info.id == collider.info.id)
			i = sCapsules.erase(i);
	}
}

void Collision::Clear()
{
	sSpheres.clear();
	sCapsules.clear();
	sId = 0;
}

std::vector<HitPair> Collision::Collide()
{
	return {};
}