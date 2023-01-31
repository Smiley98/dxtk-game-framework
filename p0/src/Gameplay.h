#pragma once
#include "Entity.h"
#include "Curves.h"
struct Components;

void Seek(Entity target, Entity seeker, float maxSpeed, Components& components);
void Flee(Entity target, Entity seeker, float maxSpeed, Components& components);
void Pursue(Entity target, Entity seeker, float maxSpeed, float dt, Components& components);
void Evade(Entity target, Entity seeker, float maxSpeed, float dt, Components& components);
void Arrive(Entity target, Entity seeker, float maxSpeed, float dt, Components& components);
void Wander(Entity seeker, float maxSpeed, float radius, Components& components);

void FollowPath(
	float dt, float lv, float& distance, size_t& point, size_t& sample,
	const Points& points, const SpeedTable& speedTable, Entity entity, Components& components);
