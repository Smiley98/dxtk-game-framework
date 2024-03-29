#pragma once
#include "ComponentHash.h"
#include "SimpleMath.h"

struct Building
{
	enum Type : size_t
	{
		TD,
		APARTMENT,
		BMO,
		CONDO,
		DUPLEX,
		OFFICE,
		PENTA,
		PINK,
		COUNT
	} type;

	DirectX::SimpleMath::Color color = { 0.75f, 0.75f, 0.75f };
	float durability;

	static uint32_t Hash();
};
