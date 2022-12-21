#include "pch.h"
#include "KinematicsSystem.h"
#include "Components.h"

namespace Kinematics
{
	// Create a gravity component that stores a gravitation force to apply to the entity's kinematic.
	// Apply said acceleration within GravitySystem::Update() (which Kinematics::Update() will then apply).
	void Update(Components& components, float dt, float timeSteps)
	{
		for (size_t i = 0; i < components.bodies.Count(); i++)
		{
			Entity entity = components.bodies.GetEntity(i);

			assert(components.transforms.GetComponent(entity) != nullptr);
			assert(&components.bodies[i] != nullptr);

			Transform& transform = *components.transforms.GetComponent(entity);
			Kinematic& body = components.bodies[i];

			// "Semi-implicit Euler integration" -- cheapest energy-conserving system
			// v2 = v1 + a(t)
			// p2 = p1 + v2(t) + 0.5a(t^2)
			//body.vel += body.acc * dt;
			//transform.DeltaTranslate(body.vel * dt + 0.5f * body.acc * dt * dt);

			body.vel += body.acc * dt;
			transform.DeltaTranslate(transform.Forward() * (body.vel * dt + 0.5f * body.acc * dt * dt));
		}

		for (size_t i = 0; i < components.bodies3.Count(); i++)
		{
			Entity entity = components.bodies3.GetEntity(i);

			assert(components.transforms.GetComponent(entity) != nullptr);
			assert(&components.bodies3[i] != nullptr);

			Transform& transform = *components.transforms.GetComponent(entity);
			Kinematic3& body = components.bodies3[i];

			// "Semi-implicit Euler integration" -- cheapest energy-conserving system
			// v2 = v1 + a(t)
			// p2 = p1 + v2(t) + 0.5a(t^2)
			body.vel += body.acc * dt;
			transform.DeltaTranslate(body.vel * dt + 0.5f * body.acc * dt * dt);
		}
	}
}
