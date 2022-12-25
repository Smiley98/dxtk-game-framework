#pragma once
#include "Transform.h"
#include "Kinematic.h"
#include "Kinematic3.h"

namespace Kinematics
{
	Vector3 Simulate3(Kinematic3& body, float dt, size_t steps = 1);
	float Simulate(Kinematic& body, float dt, size_t steps = 1);
}
