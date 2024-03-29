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
void Wander2d(Entity seeker, float maxSpeed, float radius, Components& components);

void FollowPath(float dt, float lv, Spline& spline, Entity entity, Components& components);
