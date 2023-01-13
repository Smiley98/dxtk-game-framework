#include "Geometry.h"
#include "SimpleMath.h"

void BoundY(Capsule& capsule, const DirectX::SimpleMath::Vector3& bounds)
{
	float r = bounds.x;
	float hh = bounds.y - r;
	capsule.r = r;
	capsule.hh = hh;
}

void BoundZ(Capsule& capsule, const DirectX::SimpleMath::Vector3& bounds)
{
	float r = bounds.x;
	float hh = bounds.z - r;
	capsule.r = r;
	capsule.hh = hh;
}

uint32_t Sphere::Hash()
{
	return ComponentHash::SPHERE;
}

uint32_t Capsule::Hash()
{
	return ComponentHash::CAPSULE;
}
