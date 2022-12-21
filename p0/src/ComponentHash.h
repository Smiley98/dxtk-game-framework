#pragma once
#include <cstdint>

namespace ComponentHash
{
	constexpr uint32_t NONE = 0;
	constexpr uint32_t TRANSFORM = 1;

	constexpr uint32_t KINEMATIC = TRANSFORM << 1;
	constexpr uint32_t KINEMATIC3 = KINEMATIC << 1;

	constexpr uint32_t SPHERE = KINEMATIC3 << 1;
	constexpr uint32_t CAPSULE = SPHERE << 1;

	constexpr uint32_t BUILDING = CAPSULE << 1;
	constexpr uint32_t PLAYER = BUILDING << 1;

	constexpr uint32_t IDENTIFIER = PLAYER << 1;
}
