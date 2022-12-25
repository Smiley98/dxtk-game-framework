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
		assert(components.bodies3.GetComponent(seeker) != nullptr);

		components.bodies3.GetComponent(seeker)->acc = Seek(
			components.transforms.GetComponent(target)->Translation(),
			components.transforms.GetComponent(seeker)->Translation(),
			components.bodies3.GetComponent(seeker)->vel, maxSpeed
		);
	}
	void Flee(Entity target, Entity seeker, float maxSpeed, Components& components)
	{
		assert(components.transforms.GetComponent(seeker) != nullptr);
		assert(components.transforms.GetComponent(target) != nullptr);
		assert(components.bodies3.GetComponent(seeker) != nullptr);
		
		components.bodies3.GetComponent(seeker)->acc = Flee(
			components.transforms.GetComponent(target)->Translation(),
			components.transforms.GetComponent(seeker)->Translation(),
			components.bodies3.GetComponent(seeker)->vel, maxSpeed
		);
	}

	// Cannot pursue/evade van with current model since van is 1D.
	void Pursue(Entity target, Entity seeker, float maxSpeed, Components& components)
	{
	}

	void Evade(Entity target, Entity seeker, float maxSpeed, Components& components)
	{
	}
}
