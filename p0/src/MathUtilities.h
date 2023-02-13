#pragma once
#include "Geometry.h"
#include <vector>
#include <cstdlib>
#undef min
#undef max

namespace DirectX
{
	constexpr float XM_DEGREES = 180.0f / DirectX::XM_PI;
	constexpr float XM_RADIANS = DirectX::XM_PI / 180.0f;
}

inline float Random(float min, float max)
{
	return min + (rand() / ((float)RAND_MAX / (max - min)));
}

inline float Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

inline float InverseLerp(float n, float a, float b)
{
	return (n - a) / (b - a);
}

inline Vector3 Direction(const Vector3& A, const Vector3& B)
{
	Vector3 direction = B - A;
	direction.Normalize();
	return direction;
}

// Projects point P along line AB
inline Vector3 Project(const Vector3& a, const Vector3& b, const Vector3& p)
{
	Vector3 AB = b - a;
	float t = (p - a).Dot(AB) / AB.Dot(AB);
	return a + std::min(std::max(t, 0.0f), 1.0f) * AB;
}

inline Vector3 Project(const Line& line, const Vector3& point)
{
	return Project(line.start, line.end, point);
}

inline Vector3 NearestProjection(const Vector3& position, const std::vector<Line>& lines, size_t& index)
{
	Vector3 nearest;
	float shortestDistance = std::numeric_limits<float>::max();
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vector3 projection = Project(lines[i], position);
		float distance = (position - projection).LengthSquared();
		if (distance < shortestDistance)
		{
			shortestDistance = distance;
			nearest = projection;
			index = i;
		}
	}
	return nearest;
}

inline Vector3 WorldToScreen(const Vector3& worldPoint, const Space& space)
{
	return space.viewport.Project(worldPoint, space.proj, space.view, Matrix::Identity);
};

inline Vector3 ScreenToWorld(const Vector3& screenPoint, const Space& space)
{
	return space.viewport.Unproject(screenPoint, space.proj, space.view, Matrix::Identity);
}

inline Vector3 RandomSpherePoint(float radius)
{
	float z = Random(-1.0f, 1.0f);
	float t = Random(0.0f, DirectX::XM_2PI);
	float r = sqrtf(1.0f - z * z);
	return Vector3(r * cosf(t), r * sinf(t), z ) * radius;
}

// Random point along xy-plane (z = 0.0)
inline Vector3 RandomCirclePoint(float radius)
{
	float t = Random(0.0f, DirectX::XM_2PI);
	float u = Random(-1.0f, 1.0f) + Random(-1.0f, 1.0f);
	float r = u > 1.0f ? 2.0f - u : u;
	return Vector3(r * cosf(t), r * sinf(t), 0.0f);
}

// Not very practical inverse-lerping anything other than scalars
//template<typename T>
//T InverseLerp(const T& n, const T& a, const T& b)
//{
//	return (n - a) / (b - a);
//}
//
//inline Vector3 InverseLerp(const Vector3& n, const Vector3& a, const Vector3& b)
//{
//	return (n - a) / (b - a);
//}
