#include "Collision2.h"
#include "Components.h"

namespace Collision2
{
	void Collide(const std::vector<Entity>& entities, const Components& components, std::vector<HitPair>& collisions)
	{
		std::vector<Collision2::SphereCollider> staticSpheres;
		std::vector<Collision2::SphereCollider> dynamicSpheres;
		std::vector<Collision2::CapsuleCollider> staticCapsules;
		std::vector<Collision2::CapsuleCollider> dynamicCapsules;

		// Can avoid these scary casts if we use only capsules although its nice to see multi-geometry implementation
		for (Entity entity : entities)
		{
			const Transform3* transform = components.transforms.GetComponent(entity);
			const Geometry* collider = components.colliders.GetComponent(entity);
			if (collider != nullptr)
			{
				if (collider->sphere)
				{
					Collision2::SphereCollider sphereCollider(entity, *transform, *ToSphere(collider));
					if (sphereCollider.geometry.dynamic)
						dynamicSpheres.push_back(sphereCollider);
					else
						staticSpheres.push_back(sphereCollider);
				}
				else
				{
					Collision2::CapsuleCollider capsuleCollider(entity, *transform, *ToCapsule(collider));
					if (capsuleCollider.geometry.dynamic)
						dynamicCapsules.push_back(capsuleCollider);
					else
						staticCapsules.push_back(capsuleCollider);
				}
			}
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
}