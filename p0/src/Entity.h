#pragma once
#include <cstdint>

namespace
{
	using Entity = uint32_t;
	static constexpr Entity INVALID_ENTITY = 0;
}

Entity CreateEntity()
{
	static Entity entity = 0;
	return ++entity;
}
