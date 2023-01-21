#include "Collider.h"
#include "SimpleMath.h"

void BoundY(Collider& collider, const DirectX::SimpleMath::Vector3& bounds)
{
	assert(collider.type == Collider::CAPSULE);
	float r = bounds.x;
	float hh = bounds.y - r;
	collider.r = r;
	collider.hh = hh;
}

void BoundZ(Collider& collider, const DirectX::SimpleMath::Vector3& bounds)
{
	assert(collider.type == Collider::CAPSULE);
	float r = bounds.x;
	float hh = bounds.z - r;
	collider.r = r;
	collider.hh = hh;
}

uint32_t Collider::Hash()
{
	return ComponentHash::COLLIDER;
}