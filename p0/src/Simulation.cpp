#include "Simulation.h"

namespace Kinematics
{
	//void Simulate3(const Vector3& acceleration, const Vector3& inVelocity, const Vector3& inPosition,
	//	Vector3& outVelocity, Vector3& outPosition, float dt, size_t steps)
	//{
	//
	//}
	//
	//void Simulate(float acceleration, float inVelocity, float inPosition,
	//	float& outVelocity, float& outPosition, float dt, size_t steps)
	//{
	//
	//}

	void Simulate3(Kinematic3& body, Transform& transform, float dt, size_t steps)
	{
		for (size_t i = 0; i < steps; i++)
		{
			body.vel += body.acc * dt;
			transform.DeltaTranslate(body.vel * dt + 0.5f * body.acc * dt * dt);

			Vector3 forward;
			body.vel.Normalize(forward);
			transform.Orientate(forward);
		}
	}

	void Simulate(Kinematic& body, float& distance, float dt, size_t steps)
	{
		for (size_t i = 0; i < steps; i++)
		{
			body.vel += body.acc * dt;
			distance += body.vel * dt + 0.5f * body.acc * dt * dt;
		}
	}
}
