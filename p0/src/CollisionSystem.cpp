#include "pch.h"
#include "CollisionSystem.h"
#include "PlayerSystem.h"
#include "Colliders.h"
#include "Components.h"
#include "Utility.h"
#include <assert.h>

namespace Collision
{
	void OnPlayerPlayer(Entity playerA, Entity playerB, const Vector3& mtv, Components& components);
	void OnPlayerBuilding(Entity player, Entity building, const Vector3& mtv, Components& components);
	void OnPlayerProjectile(Entity player, Entity projectile, const Vector3& mtv, Components& components);
	void OnBuildingProjectile(Entity building, Entity projectile, const Vector3& mtv, Components& components);

	void Collect(const Components& components, std::vector<HitPair>& collisions);
	void Resolve(Components& components, const std::vector<HitPair>& collisions);

	void Update(Components& components)
	{
		std::vector<HitPair> collisions;
		Collect(components, collisions);
		Resolve(components, collisions);
	}

	// Passing Components is more convenient than passing spheres and capsules directly.
	// Can modify this later if we need to hit-test specific groups of geometry rather than all geometry.
	void Collect(const Components& components, std::vector<HitPair>& collisions)
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

	void Resolve(Components& components, const std::vector<HitPair>& collisions)
	{
		for (const HitPair& collision : collisions)
		{
			// Might want to negate mtv so that it always resolves player
			Tags::Tag* tagA = components.tags.GetComponent(collision.hits[0]);
			Tags::Tag* tagB = components.tags.GetComponent(collision.hits[1]);
			assert(tagA != nullptr && tagB != nullptr);

			if (*tagA == Tags::PLAYER && *tagB == Tags::PLAYER)
			{
				OnPlayerPlayer(collision.hits[0], collision.hits[1], collision.mtv, components);
			}

			else if (*tagA == Tags::PLAYER && *tagB == Tags::BUILDING)
			{
				OnPlayerBuilding(collision.hits[0], collision.hits[1], collision.mtv, components);
			}
			else if (*tagA == Tags::BUILDING && *tagB == Tags::PLAYER)
			{
				OnPlayerBuilding(collision.hits[1], collision.hits[0], collision.mtv, components);
			}

			else if (*tagA == Tags::PLAYER && *tagB == Tags::PROJECTILE)
			{
				OnPlayerProjectile(collision.hits[0], collision.hits[1], collision.mtv, components);
			}
			else if (*tagA == Tags::PROJECTILE && *tagB == Tags::PLAYER)
			{
				OnPlayerProjectile(collision.hits[1], collision.hits[0], collision.mtv, components);
			}

			else if (*tagA == Tags::BUILDING && *tagB == Tags::PROJECTILE)
			{
				OnBuildingProjectile(collision.hits[0], collision.hits[1], collision.mtv, components);
			}
			else if (*tagA == Tags::PROJECTILE && *tagB == Tags::BUILDING)
			{
				OnBuildingProjectile(collision.hits[1], collision.hits[0], collision.mtv, components);
			}

			else
			{
				Print("***ERROR -- UNHANDLED COLLISION***");
			}

			//std::vector<HitPair> player_player;
			//std::vector<HitPair> player_building;
			//std::vector<HitPair> player_projectile;
			//std::vector<HitPair> building_projectile;
			//for (Entity entity : collision.hits)
			//{
			//	Tags::Tag* tag = components.tags.GetComponent(entity);
			//	if (tag != nullptr)
			//	{
			//		// TODO -- invoke callbacks instead of handling collisions directly 
			//		switch (*tag)
			//		{
			//		case Tags::PLAYER:
			//			components.transforms.GetComponent(entity)->DeltaTranslate(collision.mtv);
			//			break;
			//		case Tags::BUILDING:
			//			components.buildings.GetComponent(entity)->durability -= 10.0f;
			//			break;
			//		case Tags::PROJECTILE:
			//			break;
			//		}
			//	}
			//}
		}
	}

	void OnPlayerPlayer(Entity playerA, Entity playerB, const Vector3& mtv, Components& components)
	{
		assert(*components.tags.GetComponent(playerA) == Tags::PLAYER);
		assert(*components.tags.GetComponent(playerB) == Tags::PLAYER);

	}

	void OnPlayerBuilding(Entity player, Entity building, const Vector3& mtv, Components& components)
	{
		assert(*components.tags.GetComponent(player) == Tags::PLAYER);
		assert(*components.tags.GetComponent(building) == Tags::BUILDING);

		components.transforms.GetComponent(player)->DeltaTranslate(mtv);
		components.buildings.GetComponent(building)->durability -= 10.0f;
	}

	void OnPlayerProjectile(Entity player, Entity projectile, const Vector3& mtv, Components& components)
	{
		assert(*components.tags.GetComponent(player) == Tags::PLAYER);
		assert(*components.tags.GetComponent(projectile) == Tags::PROJECTILE);

	}

	void OnBuildingProjectile(Entity building, Entity projectile, const Vector3& mtv, Components& components)
	{
		assert(*components.tags.GetComponent(building) == Tags::BUILDING);
		assert(*components.tags.GetComponent(projectile) == Tags::PROJECTILE);

	}
}
