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
			const EntityTransform& transform = *components.transforms.GetComponent(entity);
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
			const EntityTransform& transform = *components.transforms.GetComponent(entity);
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
			if (components.identifiers.HasComponent(collision.hits[0]) &&
				components.identifiers.HasComponent(collision.hits[1]))
			{
				Tags::Tag tagA = components.identifiers.GetComponent(collision.hits[0])->tag;
				Tags::Tag tagB = components.identifiers.GetComponent(collision.hits[1])->tag;

				// Player-Player
				if (tagA == Tags::PLAYER && tagB == Tags::PLAYER)
				{
					OnPlayerPlayer(collision.hits[0], collision.hits[1], collision.mtv, components);
				}

				// Player-Building
				else if (tagA == Tags::PLAYER && tagB == Tags::BUILDING)
				{
					OnPlayerBuilding(collision.hits[0], collision.hits[1], collision.mtv, components);
				}
				else if (tagA == Tags::BUILDING && tagB == Tags::PLAYER)
				{
					OnPlayerBuilding(collision.hits[1], collision.hits[0], collision.mtv, components);
				}

				// Player-Projectile
				else if (tagA == Tags::PLAYER && tagB == Tags::PROJECTILE)
				{
					OnPlayerProjectile(collision.hits[0], collision.hits[1], collision.mtv, components);
				}
				else if (tagA == Tags::PROJECTILE && tagB == Tags::PLAYER)
				{
					OnPlayerProjectile(collision.hits[1], collision.hits[0], collision.mtv, components);
				}

				// Building-Projectile
				else if (tagA == Tags::BUILDING && tagB == Tags::PROJECTILE)
				{
					OnBuildingProjectile(collision.hits[0], collision.hits[1], collision.mtv, components);
				}
				else if (tagA == Tags::PROJECTILE && tagB == Tags::BUILDING)
				{
					OnBuildingProjectile(collision.hits[1], collision.hits[0], collision.mtv, components);
				}
			}
			else
			{
				Print("***WARNING: UNHANDLED COLLISION***");
			}
		}
	}

	void OnPlayerPlayer(Entity playerA, Entity playerB, const Vector3& mtv, Components& components)
	{
		assert(components.identifiers.GetComponent(playerA)->tag == Tags::PLAYER);
		assert(components.identifiers.GetComponent(playerB)->tag == Tags::PLAYER);
	}

	void OnPlayerBuilding(Entity player, Entity buildingCollider, const Vector3& mtv, Components& components)
	{
		assert(components.identifiers.GetComponent(player)->tag == Tags::PLAYER);
		assert(components.identifiers.GetComponent(buildingCollider)->tag == Tags::BUILDING);

		components.transforms.GetComponent(player)->DeltaTranslate(mtv);
		components.buildings.GetComponent(
			components.hierarchies.GetComponent(buildingCollider)->parent
		)->durability -= 10.0f;
	}

	void OnPlayerProjectile(Entity player, Entity projectile, const Vector3& mtv, Components& components)
	{
		assert(components.identifiers.GetComponent(player)->tag == Tags::PLAYER);
		assert(components.identifiers.GetComponent(projectile)->tag == Tags::PROJECTILE);
	}

	void OnBuildingProjectile(Entity building, Entity projectile, const Vector3& mtv, Components& components)
	{
		assert(components.identifiers.GetComponent(building)->tag == Tags::BUILDING);
		assert(components.identifiers.GetComponent(projectile)->tag == Tags::PROJECTILE);
	}
}

// Instead of re-querrying all collisions for a single entity, just resolve everything in Resolve().
/*
std::vector<Hit> All(Components& components, Entity entity)
{
	std::vector<HitPair> collisions;
	Collect(components, collisions);
}

Hit Nearest(Components& components, Entity entity)
{
	std::vector<HitPair> collisions;
	Collect(components, collisions);
}
*/
