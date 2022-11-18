#include "Collision2.h"
#include "Components.h"

namespace Collision2
{
	void Collide(const std::vector<Entity>& entities, const Components& components, std::vector<HitPair>& collisions)

	{
		std::vector<Transform3*> transforms;
		std::vector<SphereCollider*> staticSpheres;
		std::vector<SphereCollider*> dynamicSpheres;
		std::vector<CapsuleCollider*> staticCapsules;
		std::vector<CapsuleCollider*> dynamicCapsules;

		/*for (Entity entity : entities)
		{
			//transforms.push_back()
		}

		for (const SphereCollider& sphere : spheres)
		{
			if (sphere.dynamic)
				dynamicSpheres.push_back(&sphere);
			else
				staticSpheres.push_back(&sphere);
		}

		for (const CapsuleCollider& capsule : capsules)
		{
			if (capsule.dynamic)
				dynamicCapsules.push_back(&capsule);
			else
				staticCapsules.push_back(&capsule);
		}

		// Static spheres vs dynamic spheres & dynamic capsules
		for (size_t i = 0; i < staticSpheres.size(); i++)
		{
			for (size_t j = 0; j < dynamicSpheres.size(); j++)
			{
				Vector3 mtv;
				const SphereCollider& a = *staticSpheres[i];
				const SphereCollider& b = *dynamicSpheres[j];
				if (b.IsColliding(a, mtv))
				{
					collisions.push_back({ a.entity, b.entity, mtv });
				}
			}

			for (size_t j = 0; j < dynamicCapsules.size(); j++)
			{
				Vector3 mtv;
				const SphereCollider& a = *staticSpheres[i];
				const CapsuleCollider& b = *dynamicCapsules[j];
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
				const CapsuleCollider& a = *staticCapsules[i];
				const SphereCollider& b = *dynamicSpheres[j];
				if (b.IsColliding(a, mtv))
				{
					collisions.push_back({ a.entity, b.entity, mtv });
				}
			}

			for (size_t j = 0; j < dynamicCapsules.size(); j++)
			{
				Vector3 mtv;
				const CapsuleCollider& a = *staticCapsules[i];
				const CapsuleCollider& b = *dynamicCapsules[j];
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
				const SphereCollider& a = *dynamicSpheres[i];
				const CapsuleCollider& b = *dynamicCapsules[j];
				if (b.IsColliding(a, mtv))
				{
					collisions.push_back({ a.entity, b.entity, mtv });
				}
			}

			for (size_t j = 0; j < dynamicSpheres.size(); j++)
			{
				if (i == j) continue;
				Vector3 mtv;
				const SphereCollider& a = *dynamicSpheres[i];
				const SphereCollider& b = *dynamicSpheres[j];
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
				const CapsuleCollider& a = *dynamicCapsules[i];
				const CapsuleCollider& b = *dynamicCapsules[j];
				if (b.IsColliding(a, mtv))
				{
					collisions.push_back({ a.entity, b.entity, mtv });
				}
			}
		}*/
	}
}