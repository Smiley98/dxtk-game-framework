#include "Collision.h"
#include "Components.h"

// Passing Components is more convenient than passing spheres and capsules directly.
// Can modify this later if we need to hit-test specific groups of geometry rather than all geometry.
void Collide(const Components& components, std::vector<HitPair>& collisions)
{
	std::vector<SphereCollider> staticSpheres;
	std::vector<SphereCollider> dynamicSpheres;
	std::vector<CapsuleCollider> staticCapsules;
	std::vector<CapsuleCollider> dynamicCapsules;

	// Separate static vs dynamic spheres
	for (size_t i = 0; i < components.spheres.Count(); i++)
	{
		const Entity entity = components.spheres.GetEntity(i);
		const Transform& transform = *components.transforms.GetComponent(entity);
		const Sphere& sphere = components.spheres[i];

		SphereCollider collider(entity, transform, sphere);
		if (sphere.dynamic)
			dynamicSpheres.push_back(collider);
		else
			staticSpheres.push_back(collider);
	}

	// Separate static vs dynamic capsules
	for (size_t i = 0; i < components.capsules.Count(); i++)
	{
		const Entity entity = components.capsules.GetEntity(i);
		const Transform& transform = *components.transforms.GetComponent(entity);
		const Capsule& capsule = components.capsules[i];

		CapsuleCollider collider(entity, transform, capsule);
		if (capsule.dynamic)
			dynamicCapsules.push_back(collider);
		else
			staticCapsules.push_back(collider);
	}

	// Static spheres vs dynamic spheres & dynamic capsules
	for (size_t i = 0; i < staticSpheres.size(); i++)
	{
		for (size_t j = 0; j < dynamicSpheres.size(); j++)
		{
			Vector3 mtv;
			const SphereCollider& a = staticSpheres[i];
			const SphereCollider& b = dynamicSpheres[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.entity, b.entity, mtv });
			}
		}

		for (size_t j = 0; j < dynamicCapsules.size(); j++)
		{
			Vector3 mtv;
			const SphereCollider& a = staticSpheres[i];
			const CapsuleCollider& b = dynamicCapsules[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.entity, b.entity, mtv });
			}
		}
	}

	// Static capsules vs dynamic spheres & dynamic capsules
	for (size_t i = 0; i < staticCapsules.size(); i++)
	{
		for (size_t j = 0; j < dynamicSpheres.size(); j++)
		{
			Vector3 mtv;
			const CapsuleCollider& a = staticCapsules[i];
			const SphereCollider& b = dynamicSpheres[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.entity, b.entity, mtv });
			}
		}

		for (size_t j = 0; j < dynamicCapsules.size(); j++)
		{
			Vector3 mtv;
			const CapsuleCollider& a = staticCapsules[i];
			const CapsuleCollider& b = dynamicCapsules[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.entity, b.entity, mtv });
			}
		}
	}

	// Dynamic spheres vs dynamic spheres & dynamic capsules
	for (size_t i = 0; i < dynamicSpheres.size(); i++)
	{
		for (size_t j = 0; j < dynamicCapsules.size(); j++)
		{
			Vector3 mtv;
			const SphereCollider& a = dynamicSpheres[i];
			const CapsuleCollider& b = dynamicCapsules[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.entity, b.entity, mtv });
			}
		}

		for (size_t j = 0; j < dynamicSpheres.size(); j++)
		{
			if (i == j) continue;
			Vector3 mtv;
			const SphereCollider& a = dynamicSpheres[i];
			const SphereCollider& b = dynamicSpheres[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.entity, b.entity, mtv });
			}
		}
	}

	// Dynamic capsules vs dynamic capsules
	for (size_t i = 0; i < dynamicCapsules.size(); i++)
	{
		for (size_t j = 0; j < dynamicCapsules.size(); j++)
		{
			if (i == j) continue;
			Vector3 mtv;
			const CapsuleCollider& a = dynamicCapsules[i];
			const CapsuleCollider& b = dynamicCapsules[j];
			if (b.IsColliding(a, mtv))
			{
				collisions.push_back({ a.entity, b.entity, mtv });
			}
		}
	}
}