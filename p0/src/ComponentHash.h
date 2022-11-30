#pragma once
#include <cstdint>

//enum ComponentHash : uint32_t
//{
//	NONE = 0,
//	TRANSFORM = 1,
//	SPHERE = TRANSFORM << 1,
//	CAPSULE = SPHERE << 1,
//	KINEMATIC = CAPSULE << 1,
//	BUILDING = KINEMATIC << 1
//};

namespace ComponentHash
{
	constexpr uint32_t NONE = 0;
	constexpr uint32_t TRANSFORM = 1;
	constexpr uint32_t SPHERE = TRANSFORM << 1;
	constexpr uint32_t CAPSULE = SPHERE << 1;
	constexpr uint32_t KINEMATIC = CAPSULE << 1;
	constexpr uint32_t BUILDING = KINEMATIC << 1;
}
