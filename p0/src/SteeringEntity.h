#pragma once
#include "Entity.h"

struct Components;
namespace Steering
{
	void Arrive(Entity target, Entity seeker, float maxSpeed, float dt, Components& components);
	void Wander(Entity seeker, float maxSpeed, float radius, Components& components);

	void Seek(Entity target, Entity seeker, float maxSpeed, Components& components);
	void Flee(Entity target, Entity seeker, float maxSpeed, Components& components);
	void Pursue(Entity target, Entity seeker, float maxSpeed, float dt, Components& components);
	void Evade(Entity target, Entity seeker, float maxSpeed, float dt, Components& components);
}