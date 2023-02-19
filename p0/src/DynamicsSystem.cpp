#include "pch.h"
#include "DynamicsSystem.h"
#include "Dynamics.h"
#include "Components.h"

namespace Dynamics
{
	// If we need gravity, make a separate system that applies constant downwards acceleration if rb.gravity
	void Update(Components& components, float dt, size_t steps)
	{
		for (size_t i = 0; i < components.rigidbodies.Count(); i++)
		{
			Entity entity = components.rigidbodies.GetEntity(i);
			if (components.players.HasComponent(entity)) continue; // Player uses custom physics

			EntityTransform& transform = *components.transforms.Get(entity);
			Rigidbody& body = components.rigidbodies[i];

			Vector3 displacement;
			for (size_t i = 0; i < steps; i++)
			{
				body.velocity += body.acceleration * dt;
				displacement += Integrate(body.velocity, body.acceleration, dt);
			}
			transform.DeltaTranslate(displacement);

			Vector3 forward = body.velocity;
			forward.Normalize();
			transform.Orientate(forward);
		}
	}
}
