#pragma once
#include "Entity.h"

struct Components;
namespace Steering
{
	void Seek(Entity target, Entity seeker, float maxSpeed, Components& components);
	void Flee(Entity target, Entity seeker, float maxSpeed, Components& components);
	void Pursue(Entity target, Entity seeker, float maxSpeed, Components& components);
	void Evade(Entity target, Entity seeker, float maxSpeed, Components& components);
}