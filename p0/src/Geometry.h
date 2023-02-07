#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
// Only used in DebugRenderer at the moment because it doesn't make sense to couple geometry with position for collisions.

struct Sphere
{
	Vector3 position;
	float radius = 0.0f;
};

struct Capsule
{
	Vector3 position;
	Vector3 direction;
	float radius = 0.0f;
	float halfHeight = 0.0f;
};

struct Box
{
	Vector3 position;
	Vector3 direction;
	Vector3 extents;
};

struct Line
{
	Vector3 start;
	Vector3 end;
};

struct FoV
{
	Vector3 position;
	Vector3 direction;
	Vector3 target;
	float length = 0.0f;
	float degrees = 0.0f;
};

//struct AABB
//{
//	Vector3 position;
//	Vector3 extents;
//};
//
//struct OOBB
//{
//	Vector3 position;
//	Vector3 direction;
//	Vector3 extents;
//};
