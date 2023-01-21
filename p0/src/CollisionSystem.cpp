#include "pch.h"
#include "CollisionSystem.h"
#include "CollisionMath.h"
#include "PlayerSystem.h"
#include "Colliders.h"
#include "Hits.h"
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

	// mtv resolves b from a
	bool IsColliding(Entity a, Entity b, Vector3& mtv, Components& components)
	{
		EntityTransform& tA = *components.transforms.GetComponent(a);
		EntityTransform& tB = *components.transforms.GetComponent(b);
		Collider& cA = *components.colliders.GetComponent(a);
		Collider& cB = *components.colliders.GetComponent(b);
		bool collision = false;

		switch (cA.type)
		{
		case Collider::SPHERE:
			switch (cB.type)
			{
			case Collider::SPHERE:
				collision = SphereSphere(
					tA.WorldPosition(), tB.WorldPosition(),
					cA.r, cB.r,
				mtv);
				break;

			case Collider::CAPSULE:
				collision = SphereCapsule(
					tA.WorldPosition(), tB.WorldPosition(), tB.WorldForward(),
					cA.r, cB.r, cB.hh,
				mtv);
				break;

			//case Collider::BOX:
			//	break;
			}
			break;

		case Collider::CAPSULE:
			switch (cB.type)
			{
			case Collider::SPHERE:
				collision = SphereCapsule(
					tB.WorldPosition(), tA.WorldPosition(), tA.WorldForward(),
					cA.r, cB.r, cB.hh,
				mtv);
				mtv = -mtv;
				break;

			case Collider::CAPSULE:
				collision = CapsuleCapsule(
					tA.WorldPosition(), tB.WorldPosition(),
					tA.WorldForward(), tB.WorldForward(),
					cA.r, cB.r, cA.hh, cB.hh,
				mtv);
				break;

			//case Collider::BOX:
			//	break;
			}
			break;
		
		//case Collider::BOX:
		//	switch (cB.type)
		//	{
		//	case Collider::SPHERE:
		//		break;
		//
		//	case Collider::CAPSULE:
		//		break;
		//
		//	case Collider::BOX:
		//		break;
		//	}
		//	break;
		}

		return collision;
	}

	void Update(Components& components)
	{
		std::vector<HitPair> collisions;
		Collect(components, collisions);
		Resolve(components, collisions);
	}

	void Collect(const Components& components, std::vector<HitPair>& collisions)
	{
		std::vector<Entity> staticSpheres;
		std::vector<Entity> dynamicSpheres;
		std::vector<Entity> staticCapsules;
		std::vector<Entity> dynamicCapsules;
		//std::vector<Entity> staticBoxes;
		//std::vector<Entity> dynamicBoxes;

		// Sort based on movement and geometry categories
		for (size_t i = 0; i < components.colliders.Count(); i++)
		{
			const Collider& collider = components.colliders[i];
			switch (collider.type)
			{
			case Collider::SPHERE:
				if (collider.dynamic)
					dynamicSpheres.push_back(components.colliders.GetEntity(i));
				else
					staticSpheres.push_back(components.colliders.GetEntity(i));
				break;

			case Collider::CAPSULE:
				if (collider.dynamic)
					dynamicCapsules.push_back(components.colliders.GetEntity(i));
				else
					staticCapsules.push_back(components.colliders.GetEntity(i));
				break;

			//case Collider::BOX:
			//	if (collider.dynamic)
			//		dynamicBoxes.push_back(components.colliders.GetEntity(i));
			//	else
			//		staticBoxes.push_back(components.colliders.GetEntity(i));
			//	break;
			}
		}

		// Static spheres vs dynamic spheres & dynamic capsules
		for (Entity a : staticSpheres)
		{
			for (Entity b : dynamicSpheres)
			{
				const EntityTransform& tA = *components.transforms.GetComponent(a);
				const EntityTransform& tB = *components.transforms.GetComponent(b);
				const Collider& cA = *components.colliders.GetComponent(a);
				const Collider& cB = *components.colliders.GetComponent(b);

				Vector3 mtv;
				if (SphereSphere(
					tA.WorldPosition(), tB.WorldPosition(),
					cA.r, cB.r,
				mtv))
					collisions.push_back({ a, b, mtv });
			}

			for (Entity b : dynamicCapsules)
			{
				const EntityTransform& tA = *components.transforms.GetComponent(a);
				const EntityTransform& tB = *components.transforms.GetComponent(b);
				const Collider& cA = *components.colliders.GetComponent(a);
				const Collider& cB = *components.colliders.GetComponent(b);

				Vector3 mtv;
				if (SphereCapsule(
					tA.WorldPosition(), tB.WorldPosition(), tB.WorldForward(),
					cA.r, cB.r, cB.hh,
				mtv))
					collisions.push_back({ a, b, mtv });
			}
		}

		// Static capsules vs dynamic spheres & dynamic capsules
		for (Entity a : staticCapsules)
		{
			for (Entity b : dynamicSpheres)
			{
				const EntityTransform& tA = *components.transforms.GetComponent(a);
				const EntityTransform& tB = *components.transforms.GetComponent(b);
				const Collider& cA = *components.colliders.GetComponent(a);
				const Collider& cB = *components.colliders.GetComponent(b);

				Vector3 mtv;
				if (SphereCapsule(
					tB.WorldPosition(), tA.WorldPosition(), tA.WorldForward(),
					cB.r, cA.r, cA.hh,
				mtv))
					collisions.push_back({ a, b, -mtv });
			}

			for (Entity b : dynamicCapsules)
			{
				const EntityTransform& tA = *components.transforms.GetComponent(a);
				const EntityTransform& tB = *components.transforms.GetComponent(b);
				const Collider& cA = *components.colliders.GetComponent(a);
				const Collider& cB = *components.colliders.GetComponent(b);

				Vector3 mtv;
				if (CapsuleCapsule(
					tA.WorldPosition(), tB.WorldPosition(),
					tA.WorldForward(), tB.WorldForward(),
					cA.r, cB.r, cA.hh, cB.hh,
				mtv))
					collisions.push_back({ a, b, mtv });
			}
		}

		// Dynamic spheres vs dynamic spheres & dynamic capsules
		for (Entity a : dynamicSpheres)
		{
			for (Entity b : dynamicSpheres)
			{
				if (a == b) continue;

				const EntityTransform& tA = *components.transforms.GetComponent(a);
				const EntityTransform& tB = *components.transforms.GetComponent(b);
				const Collider& cA = *components.colliders.GetComponent(a);
				const Collider& cB = *components.colliders.GetComponent(b);

				Vector3 mtv;
				if (SphereSphere(
					tA.WorldPosition(), tB.WorldPosition(),
					cA.r, cB.r,
				mtv))
					collisions.push_back({ a, b, mtv });
			}

			for (Entity b : dynamicCapsules)
			{
				const EntityTransform& tA = *components.transforms.GetComponent(a);
				const EntityTransform& tB = *components.transforms.GetComponent(b);
				const Collider& cA = *components.colliders.GetComponent(a);
				const Collider& cB = *components.colliders.GetComponent(b);

				Vector3 mtv;
				if (SphereCapsule(
					tA.WorldPosition(), tB.WorldPosition(), tB.WorldForward(),
					cA.r, cB.r, cB.hh,
				mtv))
					collisions.push_back({ a, b, mtv });
			}
		}

		// Dynamic capsules vs dynamic capsules
		for (Entity a : dynamicCapsules)
		{
			for (Entity b : dynamicCapsules)
			{
				if (a == b) continue;

				const EntityTransform& tA = *components.transforms.GetComponent(a);
				const EntityTransform& tB = *components.transforms.GetComponent(b);
				const Collider& cA = *components.colliders.GetComponent(a);
				const Collider& cB = *components.colliders.GetComponent(b);

				Vector3 mtv;
				if (CapsuleCapsule(
					tA.WorldPosition(), tB.WorldPosition(),
					tA.WorldForward(), tB.WorldForward(),
					cA.r, cB.r, cA.hh, cB.hh,
				mtv))
					collisions.push_back({ a, b, mtv });
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
		components.buildings.GetComponent(Root(buildingCollider, components))->durability -= 10.0f;
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

/*
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

void Collect2(const Components& components, std::vector<HitPair>& collisions)
{
	std::vector<Entity> staticSpheres;
	std::vector<Entity> dynamicSpheres;
	std::vector<Entity> staticCapsules;
	std::vector<Entity> dynamicCapsules;

	// Separate static vs dynamic spheres
	for (size_t i = 0; i < components.spheres.Count(); i++)
	{
		if (components.spheres[i].dynamic)
			dynamicSpheres.push_back(components.spheres.GetEntity(i));
		else
			staticSpheres.push_back(components.spheres.GetEntity(i));
	}

	// Separate static vs dynamic capsules
	for (size_t i = 0; i < components.capsules.Count(); i++)
	{
		if (components.capsules[i].dynamic)
			dynamicCapsules.push_back(components.capsules.GetEntity(i));
		else
			staticCapsules.push_back(components.capsules.GetEntity(i));
	}

	// Static spheres vs dynamic spheres & dynamic capsules
	for (Entity a : staticSpheres)
	{
		for (Entity b : dynamicSpheres)
		{
			Vector3 mtv;
			if (SphereSphere(
				*components.transforms.GetComponent(a),
				*components.transforms.GetComponent(b),
				*components.spheres.GetComponent(a),
				*components.spheres.GetComponent(b),
				mtv))
				collisions.push_back({ a, b, mtv });
		}

		for (Entity b : dynamicCapsules)
		{
			Vector3 mtv;
			if (SphereCapsule(
				*components.transforms.GetComponent(a),
				*components.transforms.GetComponent(b),
				*components.spheres.GetComponent(a),
				*components.capsules.GetComponent(b),
				mtv))
				collisions.push_back({ a, b, mtv });
		}
	}

	// Static capsules vs dynamic spheres & dynamic capsules
	for (Entity a : staticCapsules)
	{
		for (Entity b : dynamicSpheres)
		{
			Vector3 mtv;
			if (SphereCapsule(
				*components.transforms.GetComponent(b),
				*components.transforms.GetComponent(a),
				*components.spheres.GetComponent(b),
				*components.capsules.GetComponent(a),
				mtv))
				collisions.push_back({ a, b, -mtv });
		}

		for (Entity b : dynamicCapsules)
		{
			Vector3 mtv;
			if (CapsuleCapsule(
				*components.transforms.GetComponent(a),
				*components.transforms.GetComponent(b),
				*components.capsules.GetComponent(a),
				*components.capsules.GetComponent(b),
				mtv))
				collisions.push_back({ a, b, mtv });
		}
	}

	// Dynamic spheres vs dynamic spheres & dynamic capsules
	for (Entity a : dynamicSpheres)
	{
		for (Entity b : dynamicSpheres)
		{
			if (a == b) continue;
			Vector3 mtv;
			if (SphereSphere(
				*components.transforms.GetComponent(a),
				*components.transforms.GetComponent(b),
				*components.spheres.GetComponent(a),
				*components.spheres.GetComponent(b),
				mtv))
				collisions.push_back({ a, b, mtv });
		}

		for (Entity b : dynamicCapsules)
		{
			Vector3 mtv;
			if (SphereCapsule(
				*components.transforms.GetComponent(a),
				*components.transforms.GetComponent(b),
				*components.spheres.GetComponent(a),
				*components.capsules.GetComponent(b),
				mtv))
				collisions.push_back({ a, b, mtv });
		}
	}

	// Dynamic capsules vs dynamic capsules
	for (Entity a : dynamicCapsules)
	{
		for (Entity b : dynamicCapsules)
		{
			if (a == b) continue;
			Vector3 mtv;
			if (CapsuleCapsule(
				*components.transforms.GetComponent(a),
				*components.transforms.GetComponent(b),
				*components.capsules.GetComponent(a),
				*components.capsules.GetComponent(b),
				mtv))
				collisions.push_back({ a, b, mtv });
		}
	}
}
*/
