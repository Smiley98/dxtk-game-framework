#pragma once
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

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

struct Space
{
	Viewport viewport;
	Matrix proj;
	Matrix view;
	const float worldWidth = 1600;
	const float worldHeight = 900;
	const float worldDepth = 10000.0f;
};
