#pragma once
#include <cstdint>

namespace ComponentHash
{
	constexpr uint32_t NONE = 0;

	// Foundational Components
	constexpr uint32_t TRANSFORM = 1;
	constexpr uint32_t HIERARCHY = TRANSFORM << 1;
	constexpr uint32_t IDENTIFIER = HIERARCHY << 1;

	// Physics Components
	constexpr uint32_t RIGIDBODY = IDENTIFIER << 1;
	constexpr uint32_t COLLIDER = RIGIDBODY << 1;

	// Gameplay Components
	constexpr uint32_t STEERING = COLLIDER << 1;
	constexpr uint32_t BUILDING = STEERING << 1;
	constexpr uint32_t PLAYER = BUILDING << 1;
}
