#include "pch.h"
#include "SteeringSystem.h"
#include "CollisionSystem.h"
#include "Collider.h"
#include "Steering.h"
#include "Components.h"
#include "Gameplay.h"

namespace Steering
{
	void Update(Components& components, float dt)
	{
		for (size_t i = 0; i < components.steering.Count(); i++)
		{
			SteeringBehaviour& behaviour = components.steering[i];
			Entity entity = components.steering.GetEntity(i);
			switch (behaviour.type)
			{
			case SteeringBehaviour::SEEK:
				Seek(behaviour.target, entity, behaviour.maxSpeed, components);
				break;

			case SteeringBehaviour::FLEE:
				Flee(behaviour.target, entity, behaviour.maxSpeed, components);
				break;

			case SteeringBehaviour::PURSUE:
				Pursue(behaviour.target, entity, behaviour.maxSpeed, dt, components);
				break;

			case SteeringBehaviour::EVADE:
				Evade(behaviour.target, entity, behaviour.maxSpeed, dt, components);
				break;

			case SteeringBehaviour::ARRIVE:
				Arrive(behaviour.target, entity, behaviour.maxSpeed, dt, components);
				break;

			case SteeringBehaviour::AVOID:
				// Seek collider position + mtv if on collision course, otherwise seek target.
				// (This is fine even if the collider is a capsule because whether you're seeking the
				// actual sphere contact point, or mtv relative to the centre, you'll still evade).
				Vector3 mtv;
				Entity child = *components.hierarchies.GetComponent(entity)->children.begin();
				if (Collision::IsColliding(behaviour.target, child, mtv, components))
				{
					EntityTransform& transform = *components.transforms.GetComponent(child);
					Rigidbody& rb = *components.rigidbodies.GetComponent(entity);
					Vector3 resolvedPosition = transform.WorldPosition() + mtv;
					rb.acceleration = Seek(resolvedPosition, transform.WorldPosition(), rb.velocity, behaviour.maxSpeed);
				}
				else
				{
					Seek(behaviour.target, entity, behaviour.maxSpeed, components);
				}
				break;
			}
		}
	}
}
