#pragma once
#include "Transform.h"
#include "Kinematic.h"
#include "Kinematic3.h"

namespace Kinematics
{
	//void Simulate3(
	//	const Vector3& acceleration, const Vector3& inVelocity, const Vector3& inPosition,
	//	Vector3& outVelocity, Vector3& outPosition, float dt, size_t steps = 1);
	//
	//void Simulate(
	//	float acceleration, float inVelocity, float inPosition,
	//	float& outVelocity, float& outPosition, float dt, size_t steps = 1);

	void Simulate3(Kinematic3& body, Transform& transform, float dt, size_t steps = 1);

	void Simulate(Kinematic& body, float& distance, float dt, size_t steps = 1);
}
