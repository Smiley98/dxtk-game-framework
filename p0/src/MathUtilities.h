#pragma once
#include <DirectXMath.h>
#include <cstdlib>

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
