#pragma once
#include "Transform.h"
#include <array>

void IndexCatmull(const std::array<Vector3, 4>& points, size_t i, Vector3& p0, Vector3& p1, Vector3& p2, Vector3& p3)
{
	size_t n = points.size();
    p0 = points[(i - 1 + n) % n];
    p1 = points[i % n];
    p2 = points[(i + 1) % n];
    p3 = points[(i + 2) % n];
}
