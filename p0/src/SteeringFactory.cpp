#include "pch.h"
#include "SteeringFactory.h"
#include "Components.h"

Entity CreateSteering(Components& components, SteeringBehaviour::Type type,
	float maxSpeed, Entity target)
{
	Entity entity = CreateEntity(components);
	components.rigidbodies.Add(entity);

	SteeringBehaviour& behaviour = components.steering.Add(entity);
	behaviour.type = type;
	behaviour.maxSpeed = maxSpeed;
	behaviour.target = target;

	return entity;
}
