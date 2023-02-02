#pragma once
#include <SimpleMath.h>
#include <cstdlib>
#undef min
#undef max

using namespace DirectX::SimpleMath;

namespace DirectX
{
	XM_CONST float XM_DEGREES = 180.0f / DirectX::XM_PI;
	XM_CONST float XM_RADIANS = DirectX::XM_PI / 180.0f;
}

inline float Random(float min, float max)
{
	return min + (rand() / ((float)RAND_MAX / (max - min)));
}

inline float Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

// Projects point P along line AB
inline Vector3 Project(const Vector3& a, const Vector3& b, const Vector3& p)
{
	Vector3 AB = b - a;
	float t = (p - a).Dot(AB) / AB.Dot(AB);
	return a + std::min(std::max(t, 0.0f), 1.0f) * AB;
}

inline Vector3 RandomSpherePoint(float radius)
{
	float z = Random(-1.0f, 1.0f);
	float a = Random(0.0f, DirectX::XM_2PI);
	float r = sqrtf(1.0f - z * z);
	float x = r * cosf(a);
	float y = r * sinf(a);
	return Vector3(x, y, z ) * radius;
}
