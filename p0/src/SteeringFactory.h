#pragma once
#include "Entity.h"
#include "SteeringBehaviour.h"

struct Components;
Entity CreateSteering(Components& components, SteeringBehaviour::Type type,
	float maxSpeed, Entity target = INVALID_ENTITY);