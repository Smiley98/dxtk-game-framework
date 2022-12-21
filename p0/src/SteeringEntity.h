#pragma once
#include "Entity.h"

struct Components;
namespace Steering
{
	void Seek(Entity target, Entity seeker, float maxSpeed, Components& components);
}