#include "pch.h"
#include "SteeringSystem.h"
#include "SteeringEntity.h"
#include "Steering.h"
#include "Components.h"
#include "CollisionSystem.h"

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
				Entity child = *components.hierarchies.GetComponent(entity)->children.begin();
				Vector3 mtv;
				//bool collision = SphereSphere(
				//	*components.transforms.GetComponent(behaviour.target),
				//	*components.transforms.GetComponent(child),
				//	*components.spheres.GetComponent(behaviour.target),
				//	*components.spheres.GetComponent(child), mtv
				//);
				
				// Seek to collider position + mtv if on collision course, otherwise seek to target.
				bool collision = Collision::IsColliding(behaviour.target, child, mtv, components);
				if (collision)
				{
					Vector3 resolvedPosition = components.transforms.GetComponent(child)->WorldPosition() + mtv;
					Seek(resolvedPosition, components.transforms.GetComponent(entity)->WorldPosition(),
						components.rigidbodies.GetComponent(entity)->velocity, behaviour.maxSpeed);
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

// Not sure why ternary doesn't work here...
//Collider* collider = components.spheres.HasComponent(child) ?
//	new SphereCollider(child, childTransform, *components.spheres.GetComponent(child)) :
//	new CapsuleCollider(child, childTransform, *components.capsules.GetComponent(child));
