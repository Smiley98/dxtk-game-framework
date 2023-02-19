#include "pch.h"
#include "EntityFunctions.h"
#include "Components.h"

#include "Steering.h"
#include "Dynamics.h"

void Seek(Entity target, Entity seeker, float maxSpeed, Components& components)
{
	assert(components.transforms.HasComponent(seeker));
	assert(components.transforms.HasComponent(target));
	assert(components.rigidbodies.HasComponent(seeker));

	components.rigidbodies.Get(seeker)->acceleration = Steering::Seek(
		components.transforms.Get(target)->Translation(),
		components.transforms.Get(seeker)->Translation(),
		components.rigidbodies.Get(seeker)->velocity, maxSpeed
	);
}

void Flee(Entity target, Entity seeker, float maxSpeed, Components& components)
{
	assert(components.transforms.HasComponent(seeker));
	assert(components.transforms.HasComponent(target));
	assert(components.rigidbodies.HasComponent(seeker));

	components.rigidbodies.Get(seeker)->acceleration = Steering::Flee(
		components.transforms.Get(target)->Translation(),
		components.transforms.Get(seeker)->Translation(),
		components.rigidbodies.Get(seeker)->velocity, maxSpeed
	);
}

void Pursue(Entity target, Entity seeker, float maxSpeed, float dt, Components& components)
{
	assert(components.transforms.HasComponent(seeker));
	assert(components.transforms.HasComponent(target));
	assert(components.rigidbodies.HasComponent(seeker));
	assert(components.rigidbodies.HasComponent(target));

	components.rigidbodies.Get(seeker)->acceleration = Steering::Pursue(
		components.transforms.Get(target)->Translation(),
		components.rigidbodies.Get(target)->velocity,
		components.rigidbodies.Get(target)->acceleration,
		components.transforms.Get(seeker)->Translation(),
		components.rigidbodies.Get(seeker)->velocity, maxSpeed, dt
	);
}

void Evade(Entity target, Entity seeker, float maxSpeed, float dt, Components& components)
{
	assert(components.transforms.HasComponent(seeker));
	assert(components.transforms.HasComponent(target));
	assert(components.rigidbodies.HasComponent(seeker));
	assert(components.rigidbodies.HasComponent(target));

	components.rigidbodies.Get(seeker)->acceleration = Steering::Evade(
		components.transforms.Get(target)->Translation(),
		components.rigidbodies.Get(target)->velocity,
		components.rigidbodies.Get(target)->acceleration,
		components.transforms.Get(seeker)->Translation(),
		components.rigidbodies.Get(seeker)->velocity, maxSpeed, dt
	);
}

void Arrive(Entity target, Entity seeker, float maxSpeed, float dt, Components& components)
{
	assert(components.transforms.HasComponent(seeker));
	assert(components.transforms.HasComponent(target));
	assert(components.rigidbodies.HasComponent(seeker));

	// 1. Add steering force towards target
	Rigidbody& rb = *components.rigidbodies.Get(seeker);
	rb.acceleration = Steering::Seek(
		components.transforms.Get(target)->Translation(),
		components.transforms.Get(seeker)->Translation(),
		rb.velocity, maxSpeed
	);

	// 2. Integrate velocity then correct acceleration
	rb.velocity += rb.acceleration * dt;
	rb.acceleration = Dynamics::Decelerate(
		components.transforms.Get(target)->Translation(),
		components.transforms.Get(seeker)->Translation(),
		components.rigidbodies.Get(seeker)->velocity
	);
}

void Wander(Entity seeker, float maxSpeed, float radius, Components& components)
{
	assert(components.transforms.HasComponent(seeker));
	assert(components.rigidbodies.HasComponent(seeker));

	components.rigidbodies.Get(seeker)->acceleration = Steering::Wander(
		components.transforms.Get(seeker)->Translation(),
		components.rigidbodies.Get(seeker)->velocity,
		maxSpeed, radius
	);
}

void Wander2d(Entity seeker, float maxSpeed, float radius, Components& components)
{
	assert(components.transforms.HasComponent(seeker));
	assert(components.rigidbodies.HasComponent(seeker));

	components.rigidbodies.Get(seeker)->acceleration = Steering::Wander2d(
		components.transforms.Get(seeker)->Translation(),
		components.rigidbodies.Get(seeker)->velocity,
		maxSpeed, radius
	);
}

void FollowPath(
	float dt, float lv, Spline& spline, Entity entity, Components& components)
{
	Vector3 a = Catmull(DistanceToInterpolation(spline), spline.point, spline.points);
	spline.distance += lv * dt;
	UpdateCatmull(spline);
	Vector3 b = Catmull(DistanceToInterpolation(spline), spline.point, spline.points);

	Vector3 forward = b - a;
	forward.Normalize();
	components.transforms.Get(entity)->Translate(a);
	components.transforms.Get(entity)->Orientate(forward);
}
