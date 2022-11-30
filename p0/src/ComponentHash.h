#pragma once
#include <cstdint>

namespace ComponentHash
{
	constexpr uint32_t NONE = 0;
	constexpr uint32_t TRANSFORM = 1;
	constexpr uint32_t SPHERE = TRANSFORM << 1;
	constexpr uint32_t CAPSULE = SPHERE << 1;
	constexpr uint32_t KINEMATIC = CAPSULE << 1;
	constexpr uint32_t BUILDING = KINEMATIC << 1;
	constexpr uint32_t IDENTIFIER = BUILDING << 1;
}
