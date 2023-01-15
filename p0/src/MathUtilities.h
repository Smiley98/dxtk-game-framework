#pragma once
#include <SimpleMath.h>
#include <cstdlib>

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

inline Vector3 RandomSpherePoint(float radius)
{
	float z = Random(-1.0f, 1.0f);
	float a = Random(0.0f, DirectX::XM_2PI);
	float r = sqrtf(1.0f - z * z);
	float x = r * cosf(a);
	float y = r * sinf(a);
	return Vector3(x, y, z ) * radius;
}
