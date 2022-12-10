#include "pch.h"
#include "KinematicsSystem.h"
#include "Components.h"

namespace Kinematics
{
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
			body.hVel += body.hAcc * dt;
			transform.DeltaTranslate(transform.Forward() * (body.hVel * dt + 0.5f * body.hAcc * dt * dt));

			// No use-case yet...
			//if (body.gravity)
			//{
			//	body.vVel += body.vAcc * dt;
			//	transform.DeltaTranslate(Vector3::UnitZ * (body.vVel * dt + 0.5f * body.vAcc * dt * dt));
			//}
		}
	}
}
