#pragma once
#include <cstdint>

namespace ComponentHash
{
	constexpr uint32_t NONE = 0;
	constexpr uint32_t TRANSFORM = 1;

	constexpr uint32_t RIGIDBODY = TRANSFORM << 1;
	constexpr uint32_t SPHERE = RIGIDBODY << 1;
	constexpr uint32_t CAPSULE = SPHERE << 1;

	constexpr uint32_t BUILDING = CAPSULE << 1;
	constexpr uint32_t PLAYER = BUILDING << 1;

	constexpr uint32_t IDENTIFIER = PLAYER << 1;
}
