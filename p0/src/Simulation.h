#pragma once
#include "Transform.h"
#include "Kinematic.h"
#include "Kinematic3.h"

namespace Kinematics
{
	Vector3 Simulate3(const Vector3& pos, const Vector3& vel, const Vector3& acc, float dt);
	float Simulate3(float pos, float vel, float acc, float dt);

	Vector3 Simulate3(Kinematic3& body, float dt, size_t steps = 1);
	float Simulate(Kinematic& body, float dt, size_t steps = 1);
}
