#include "pch.h"
#include "SteeringSystem.h"
#include "SteeringEntity.h"
#include "Components.h"

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
				break;
			}
		}
	}
}
