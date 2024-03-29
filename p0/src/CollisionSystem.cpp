#include "pch.h"
#include "CollisionSystem.h"
#include "CollisionMath.h"
#include "Components.h"
#include "Utility.h"

namespace Collision
{
	struct HitPair
	{
		std::array<Entity, 2> hits { INVALID_ENTITY, INVALID_ENTITY };
		Vector3 mtv;
	};

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
			assert(collider.type != Collider::NONE);
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
				const EntityTransform& tA = *components.transforms.Get(a);
				const EntityTransform& tB = *components.transforms.Get(b);
				const Collider& cA = *components.colliders.Get(a);
				const Collider& cB = *components.colliders.Get(b);

				Vector3 mtv;
				if (SphereSphere(
					tA.WorldPosition(), tB.WorldPosition(),
					cA.r, cB.r,
				mtv))
					collisions.push_back({ a, b, mtv });
			}

			for (Entity b : dynamicCapsules)
			{
				const EntityTransform& tA = *components.transforms.Get(a);
				const EntityTransform& tB = *components.transforms.Get(b);
				const Collider& cA = *components.colliders.Get(a);
				const Collider& cB = *components.colliders.Get(b);

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
				const EntityTransform& tA = *components.transforms.Get(a);
				const EntityTransform& tB = *components.transforms.Get(b);
				const Collider& cA = *components.colliders.Get(a);
				const Collider& cB = *components.colliders.Get(b);

				Vector3 mtv;
				if (SphereCapsule(
					tB.WorldPosition(), tA.WorldPosition(), tA.WorldForward(),
					cB.r, cA.r, cA.hh,
				mtv))
					collisions.push_back({ a, b, -mtv });
			}

			for (Entity b : dynamicCapsules)
			{
				const EntityTransform& tA = *components.transforms.Get(a);
				const EntityTransform& tB = *components.transforms.Get(b);
				const Collider& cA = *components.colliders.Get(a);
				const Collider& cB = *components.colliders.Get(b);

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

				const EntityTransform& tA = *components.transforms.Get(a);
				const EntityTransform& tB = *components.transforms.Get(b);
				const Collider& cA = *components.colliders.Get(a);
				const Collider& cB = *components.colliders.Get(b);

				Vector3 mtv;
				if (SphereSphere(
					tA.WorldPosition(), tB.WorldPosition(),
					cA.r, cB.r,
				mtv))
					collisions.push_back({ a, b, mtv });
			}

			for (Entity b : dynamicCapsules)
			{
				const EntityTransform& tA = *components.transforms.Get(a);
				const EntityTransform& tB = *components.transforms.Get(b);
				const Collider& cA = *components.colliders.Get(a);
				const Collider& cB = *components.colliders.Get(b);

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

				const EntityTransform& tA = *components.transforms.Get(a);
				const EntityTransform& tB = *components.transforms.Get(b);
				const Collider& cA = *components.colliders.Get(a);
				const Collider& cB = *components.colliders.Get(b);

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
				Tags::Tag tagA = components.identifiers.Get(collision.hits[0])->tag;
				Tags::Tag tagB = components.identifiers.Get(collision.hits[1])->tag;

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
		assert(components.identifiers.Get(playerA)->tag == Tags::PLAYER);
		assert(components.identifiers.Get(playerB)->tag == Tags::PLAYER);
		components.transforms.Get(playerA)->DeltaTranslate(-mtv);
		components.transforms.Get(playerB)->DeltaTranslate(mtv);
	}

	void OnPlayerBuilding(Entity player, Entity buildingCollider, const Vector3& mtv, Components& components)
	{
		assert(components.identifiers.Get(player)->tag == Tags::PLAYER);
		assert(components.identifiers.Get(buildingCollider)->tag == Tags::BUILDING);

		components.transforms.Get(player)->DeltaTranslate(mtv);
		components.buildings.Get(Root(buildingCollider, components))->durability -= 10.0f;
	}

	void OnPlayerProjectile(Entity player, Entity projectile, const Vector3& mtv, Components& components)
	{
		assert(components.identifiers.Get(player)->tag == Tags::PLAYER);
		assert(components.identifiers.Get(projectile)->tag == Tags::PROJECTILE);
	}

	void OnBuildingProjectile(Entity building, Entity projectile, const Vector3& mtv, Components& components)
	{
		assert(components.identifiers.Get(building)->tag == Tags::BUILDING);
		assert(components.identifiers.Get(projectile)->tag == Tags::PROJECTILE);
	}

	// mtv resolves b from a
	bool IsColliding(Entity a, Entity b, Vector3& mtv, Components& components)
	{
		EntityTransform& tA = *components.transforms.Get(a);
		EntityTransform& tB = *components.transforms.Get(b);
		Collider& cA = *components.colliders.Get(a);
		Collider& cB = *components.colliders.Get(b);
		assert(cA.type != Collider::NONE && cB.type != Collider::NONE);
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
					cB.r, cA.r, cA.hh,
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

	bool IsColliding(Entity a, Entity b, Components& components)
	{
		EntityTransform& tA = *components.transforms.Get(a);
		EntityTransform& tB = *components.transforms.Get(b);
		Collider& cA = *components.colliders.Get(a);
		Collider& cB = *components.colliders.Get(b);
		assert(cA.type != Collider::NONE && cB.type != Collider::NONE);
		bool collision = false;

		switch (cA.type)
		{
		case Collider::SPHERE:
			switch (cB.type)
			{
			case Collider::SPHERE:
				collision = SphereSphere(
					tA.WorldPosition(), tB.WorldPosition(),
					cA.r, cB.r);
				break;

			case Collider::CAPSULE:
				collision = SphereCapsule(
					tA.WorldPosition(), tB.WorldPosition(), tB.WorldForward(),
					cA.r, cB.r, cB.hh);
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
					cB.r, cA.r, cA.hh);
				break;

			case Collider::CAPSULE:
				collision = CapsuleCapsule(
					tA.WorldPosition(), tB.WorldPosition(),
					tA.WorldForward(), tB.WorldForward(),
					cA.r, cB.r, cA.hh, cB.hh);
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
}
