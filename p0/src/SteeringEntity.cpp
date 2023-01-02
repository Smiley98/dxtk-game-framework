#include "pch.h"
#include "SteeringEntity.h"
#include "Steering.h"
#include "Components.h"

namespace Steering
{
	void Seek(Entity target, Entity seeker, float maxSpeed, Components& components)
	{
		assert(components.transforms.GetComponent(seeker) != nullptr);
		assert(components.transforms.GetComponent(target) != nullptr);
		assert(components.rigidbodies.GetComponent(seeker) != nullptr);

		components.rigidbodies.GetComponent(seeker)->acceleration = Seek(
			components.transforms.GetComponent(target)->Translation(),
			components.transforms.GetComponent(seeker)->Translation(),
			components.rigidbodies.GetComponent(seeker)->velocity, maxSpeed
		);
	}
	void Flee(Entity target, Entity seeker, float maxSpeed, Components& components)
	{
		assert(components.transforms.GetComponent(seeker) != nullptr);
		assert(components.transforms.GetComponent(target) != nullptr);
		assert(components.rigidbodies.GetComponent(seeker) != nullptr);
		
		components.rigidbodies.GetComponent(seeker)->acceleration = Flee(
			components.transforms.GetComponent(target)->Translation(),
			components.transforms.GetComponent(seeker)->Translation(),
			components.rigidbodies.GetComponent(seeker)->velocity, maxSpeed
		);
	}

	void Pursue(Entity target, Entity seeker, float maxSpeed, float dt, Components& components)
	{
		assert(components.transforms.GetComponent(seeker) != nullptr);
		assert(components.transforms.GetComponent(target) != nullptr);
		assert(components.rigidbodies.GetComponent(seeker) != nullptr);
		assert(components.rigidbodies.GetComponent(target) != nullptr);

		components.rigidbodies.GetComponent(seeker)->acceleration = Pursue(
			components.transforms.GetComponent(target)->Translation(),
			components.rigidbodies.GetComponent(target)->velocity,
			components.rigidbodies.GetComponent(target)->acceleration,
			components.transforms.GetComponent(seeker)->Translation(),
			components.rigidbodies.GetComponent(seeker)->velocity, maxSpeed, dt
		);
	}

	void Evade(Entity target, Entity seeker, float maxSpeed, float dt, Components& components)
	{
		assert(components.transforms.GetComponent(seeker) != nullptr);
		assert(components.transforms.GetComponent(target) != nullptr);
		assert(components.rigidbodies.GetComponent(seeker) != nullptr);
		assert(components.rigidbodies.GetComponent(target) != nullptr);

		components.rigidbodies.GetComponent(seeker)->acceleration = Evade(
			components.transforms.GetComponent(target)->Translation(),
			components.rigidbodies.GetComponent(target)->velocity,
			components.rigidbodies.GetComponent(target)->acceleration,
			components.transforms.GetComponent(seeker)->Translation(),
			components.rigidbodies.GetComponent(seeker)->velocity, maxSpeed, dt
		);
	}
}
