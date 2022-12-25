#include "pch.h"
#include "KinematicsSystem.h"
#include "Simulation.h"
#include "Components.h"

namespace Kinematics
{
	// Create a gravity component that stores a gravitation force to apply to the entity's kinematic.
	// Apply said acceleration within GravitySystem::Update() (which Kinematics::Update() will then apply).
	void Update(Components& components, float dt, size_t steps)
	{
		for (size_t i = 0; i < components.bodies.Count(); i++)
		{
			Entity entity = components.bodies.GetEntity(i);

			assert(components.transforms.GetComponent(entity) != nullptr);
			assert(&components.bodies[i] != nullptr);

			Transform& transform = *components.transforms.GetComponent(entity);
			Kinematic& body = components.bodies[i];

			// Orientation is determined externally since motion is 1D.
			transform.DeltaTranslate(transform.Forward() * Simulate(body, dt, steps));
		}

		for (size_t i = 0; i < components.bodies3.Count(); i++)
		{
			Entity entity = components.bodies3.GetEntity(i);

			assert(components.transforms.GetComponent(entity) != nullptr);
			assert(&components.bodies3[i] != nullptr);

			Transform& transform = *components.transforms.GetComponent(entity);
			Kinematic3& body = components.bodies3[i];

			// Orientate to velocity direction (linear 3D motion with no angular physics).
			Vector3 displacement = Simulate3(body, dt, steps);
			Vector3 forward = body.vel;
			forward.Normalize();
			transform.DeltaTranslate(displacement);
			transform.Orientate(forward);
		}
	}
}
