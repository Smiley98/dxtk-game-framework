#pragma once
#include "CollisionMath.h"
#include "Tags.h"
#include <vector>

// TODO: static vs dynamic (building spheres vs everything else -- may not need all 4 vectors)
	//std::vector<SphereCollider> sStaticSpheres;
	//std::vector<SphereCollider> sDynamicSpheres;
	//std::vector<CapsuleCollider> sStaticCapsules;
	//std::vector<CapsuleCollider> sDynamicCapsules;
	
//TODO: make convenience methods that only return collision boolean rather than calculating mtv in addition
//struct SphereCollider;
//struct CapsuleCollider;
//inline bool SphereCapsule(const SphereCollider& a, const CapsuleCollider& b, DirectX::SimpleMath::Vector3& mtv);

struct ColliderInfo
{
	Tag tag = NONE;
	void* data = nullptr;
	uint32_t id = 0;
};

// Inheritance is for losers (x1)
struct SphereCollider
{
	SphereCollider() = default;
	~SphereCollider() = default;

	inline bool IsColliding(const SphereCollider& collider) { return SphereSphere(collider.translation, collider.radius, translation, radius); }
	inline bool IsColliding(const SphereCollider& collider, DirectX::SimpleMath::Vector3& mtv) { return SphereSphere(collider.translation, collider.radius, translation, radius, mtv); }

	//inline bool IsColliding(const CapsuleCollider& collider, DirectX::SimpleMath::Vector3& mtv)
	//{
	//	bool isColliding = SphereCapsule(*this, collider, mtv);
	//	mtv = -mtv;
	//	return isColliding;
	//}

	DirectX::SimpleMath::Vector3 translation;
	float radius = 0.0f;

	ColliderInfo info;
};

// Inheritance is for losers (x2)
struct CapsuleCollider
{
	CapsuleCollider() = default;
	~CapsuleCollider() = default;

	inline bool IsColliding(const CapsuleCollider& collider) { return CapsuleCapsule(collider.transform, collider.halfHeight, collider.radius, transform, halfHeight, radius); }
	inline bool IsColliding(const CapsuleCollider& collider, DirectX::SimpleMath::Vector3& mtv) { return CapsuleCapsule(collider.transform, collider.halfHeight, collider.radius, transform, halfHeight, radius, mtv); }
	
	//inline bool IsColliding(const SphereCollider& collider, DirectX::SimpleMath::Vector3& mtv)
	//{
	//	return SphereCapsule(collider, *this, mtv);
	//}

	RigidTransform transform;
	float halfHeight = 0.0f;
	float radius = 0.0f;

	ColliderInfo info;
};

struct HitPair
{
	Tag tag1;
	Tag tag2;
	void* data1;
	void* data2;
	DirectX::SimpleMath::Vector3 mtv;
};

class Collision
{
public:
	static SphereCollider& Add(const DirectX::SimpleMath::Vector3& translation, float radius, Tag tag, void* data);
	static CapsuleCollider& Add(const RigidTransform& transform, float halfHeight, float radius, Tag tag, void* data);

	static void Remove(const SphereCollider& collider);
	static void Remove(const CapsuleCollider& collider);

	static void Clear();
	static std::vector<HitPair> Collide();

private:
	static std::vector<SphereCollider> sSpheres;
	static std::vector<CapsuleCollider> sCapsules;
	static uint32_t sId;
};

/*
// MTV resolves b from a
inline bool SphereCapsule(const SphereCollider& a, const CapsuleCollider& b, DirectX::SimpleMath::Vector3& mtv)
{
	using namespace DirectX::SimpleMath;
	Vector3 front = b.transform.Front();
	Vector3 cylinderTop = b.transform.Translation() + front * b.halfHeight;
	Vector3 cylinderBot = b.transform.Translation() - front * b.halfHeight;
	return SphereSphere(a, { ClosestLinePoint(cylinderBot, cylinderTop, a.translation), b.radius }, mtv);
}
*/