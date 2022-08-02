#pragma once
#include "Tags.h"
#include "Transform.h"
#include <vector>

// 3.0 because 1.0 for collider a, 1.0 for collider b, and another 1.0 for good measure ;)
constexpr float RESOLUTION = 1.0f + 3.0f * FLT_EPSILON;

// TODO: static vs dynamic (building spheres vs everything else -- may not need all 4 vectors)
	//std::vector<SphereCollider> sStaticSpheres;
	//std::vector<SphereCollider> sDynamicSpheres;
	//std::vector<CapsuleCollider> sStaticCapsules;
	//std::vector<CapsuleCollider> sDynamicCapsules;
	
//TODO: make convenience methods that only return collision boolean rather than calculating mtv in addition
struct SphereCollider;
struct CapsuleCollider;
inline bool SphereSphere(const SphereCollider& a, const SphereCollider& b, DirectX::SimpleMath::Vector3& mtv);
inline bool CapsuleCapsule(const CapsuleCollider& a, const CapsuleCollider& b, DirectX::SimpleMath::Vector3& mtv);
inline bool SphereCapsule(const SphereCollider& a, const CapsuleCollider& b, DirectX::SimpleMath::Vector3& mtv);

// Inheritance is for losers (x1)
struct SphereCollider
{
	SphereCollider() = default;
	~SphereCollider() = default;

	// Calls SphereSphere(other, this) --> resolves this
	inline bool IsColliding(const SphereCollider& other, DirectX::SimpleMath::Vector3& mtv)
	{
		return SphereSphere(other, *this, mtv);
	}

	DirectX::SimpleMath::Vector3 translation;
	float radius = 0.0f;

	Tag tag = NONE;
	void* data = nullptr;
	uint32_t id = 0;
};

// Inheritance is for losers (x2)
struct CapsuleCollider
{
	CapsuleCollider() = default;
	~CapsuleCollider() = default;

	RigidTransform mTransform;
	float halfHeight = 0.0f;
	float radius = 0.0f;

	Tag tag = NONE;
	void* data = nullptr;
	uint32_t id = 0;
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

// mtv points from a to b ie:
// let A = { 0.0, 0.0 }
// let B = { 1.0, 1.0 }
// If we want to resolve B intuitively, then we NEED to pass them as (A, B) to
// yield an mtv that pushes B towards +x +y.
// If we instead passed (B, A), then the mtv would be along -x -y which is counter-intuitive.
inline bool SphereSphere(const SphereCollider& a, const SphereCollider& b, DirectX::SimpleMath::Vector3& mtv)
{
	using namespace DirectX::SimpleMath;
	Vector3 AB = b.translation - a.translation;
	float lengthAB = AB.Length();
	float radiiAB = a.radius + b.radius;
	bool colliding = lengthAB <= radiiAB;
	if (colliding)
	{
		Vector3 direction;
		AB.Normalize(direction);
		mtv = direction * (radiiAB - lengthAB);
		mtv *= RESOLUTION;
	}
	return colliding;
}

inline bool CapsuleCapsule(const CapsuleCollider& a, const CapsuleCollider& b, DirectX::SimpleMath::Vector3& mtv)
{
	using namespace DirectX::SimpleMath;
	return true;
}

inline bool SphereCapsule(const SphereCollider& a, const CapsuleCollider& b, DirectX::SimpleMath::Vector3& mtv)
{
	using namespace DirectX::SimpleMath;
	return true;
}