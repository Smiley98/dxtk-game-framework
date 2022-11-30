#include "pch.h"
#include "Geometry.h"
#include "SimpleMath.h"

void AutoBound(Capsule& capsule, const DirectX::SimpleMath::Vector3& bounds)
{
	float r = bounds.x;
	float hh = bounds.y - r;
	capsule.r = r;
	capsule.hh = hh;
}
