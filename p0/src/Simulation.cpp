#include "Simulation.h"

// "Semi-implicit Euler integration" -- cheapest energy-conserving system
// v2 = v1 + a(t)
// p2 = p1 + v2(t) + 0.5a(t^2)
namespace Kinematics
{
	Vector3 Simulate3(Kinematic3& body, float dt, size_t steps)
	{
		Vector3 displacement;
		for (size_t i = 0; i < steps; i++)
		{
			body.vel += body.acc * dt;
			displacement += body.vel * dt + 0.5f * body.acc * dt * dt;
		}
		return displacement;
	}

	float Simulate(Kinematic& body, float dt, size_t steps)
	{
		float displacement = 0.0f;
		for (size_t i = 0; i < steps; i++)
		{
			body.vel += body.acc * dt;
			displacement += body.vel * dt + 0.5f * body.acc * dt * dt;
		}
		return displacement;
	}
}
