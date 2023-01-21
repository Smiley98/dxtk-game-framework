#include "Collider.h"
#include "SimpleMath.h"
#include "Components.h"

void AddSphere(Entity entity, float r, Components& components)
{
	Collider& collider = components.colliders.Add(entity);
	collider.type = Collider::SPHERE;
	collider.r = r;
}

void AddCapsule(Entity entity, float r, float hh, Components& components)
{
	Collider& collider = components.colliders.Add(entity);
	collider.type = Collider::CAPSULE;
	collider.r = r;
	collider.hh = hh;
}

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
