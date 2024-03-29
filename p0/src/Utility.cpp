#define _CRT_SECURE_NO_WARNINGS
#include "Utility.h"
#include <debugapi.h>

void Print(const Vector2& vector)
{
	char buffer[64];
	sprintf(buffer, "x: %f, y: %f\n", vector.x, vector.y);
	OutputDebugStringA(buffer);
}

void Print(const Vector3& vector)
{
	char buffer[64];
	sprintf(buffer, "x: %f, y: %f, z: %f\n", vector.x, vector.y, vector.z);
	OutputDebugStringA(buffer);
}

void Print(const Vector4& vector)
{
	char buffer[64];
	sprintf(buffer, "x: %f, y: %f, z: %f, w: %f \n", vector.x, vector.y, vector.z, vector.w);
	OutputDebugStringA(buffer);
}

void Print(const std::string& message)
{
	assert(message.length() < 64);
	char buffer[64];
	sprintf(buffer, "%s\n", message.c_str());
	OutputDebugStringA(buffer);
}

std::string ToString(const Vector2& vector)
{
	return "x: " + std::to_string(vector.x) +
		" y: " + std::to_string(vector.y);
}

std::string ToString(const Vector3& vector)
{
	return "x: " + std::to_string(vector.x) +
		" y: " + std::to_string(vector.y) +
		" z: " + std::to_string(vector.z);
}

std::string ToString(const Vector4& vector)
{
	return "x: " + std::to_string(vector.x) +
		" y: " + std::to_string(vector.y) +
		" z: " + std::to_string(vector.z) +
		" w: " + std::to_string(vector.w);
}
