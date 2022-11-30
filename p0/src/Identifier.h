#pragma once
#include "ComponentHash.h"
#include "Tags.h"

struct Identifier
{
	Tags::Tag tag;

	static uint32_t Hash();
};
