#pragma once
#include "SimpleMath.h"
#include "ComponentHash.h"

struct Kinematic
{
	DirectX::SimpleMath::Vector3 vel;
	DirectX::SimpleMath::Vector3 acc;

	static uint32_t Hash();
};
