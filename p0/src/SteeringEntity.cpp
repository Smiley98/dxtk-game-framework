#include "pch.h"
#include "SteeringEntity.h"
#include "Steering.h"
#include "Components.h"

namespace Steering
{
	void Seek(Entity target, Entity seeker, float maxSpeed, Components& components)
	{
		components.bodies3.GetComponent(seeker)->acc = Seek(
			components.transforms.GetComponent(target)->Translation(),
			components.transforms.GetComponent(seeker)->Translation(),
			components.bodies3.GetComponent(seeker)->vel, maxSpeed
		);
	}
}
