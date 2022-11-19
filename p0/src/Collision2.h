#pragma once
#include "CollisionMath2.h"
#include "Entity.h"
#include <vector>

struct Components;

// This file is an excellent example of everything wrong with C++ and object-oriented programming.
namespace Collision2
{
	class SphereCollider;
	class CapsuleCollider;

	struct HitPair
	{
		Entity a = INVALID_ENTITY;
		Entity b = INVALID_ENTITY;
		Vector3 mtv;
	};

	void Collide(
		const std::vector<Entity>& entities,
		const Components& components,
		std::vector<HitPair>& collisions);

	class Collider
	{
	public:
		Collider(const Entity& e, const Transform3& t) : entity(e), transform(t) {}

	protected:
		virtual bool IsColliding(const SphereCollider& collider) const = 0;
		virtual bool IsColliding(const CapsuleCollider& collider) const = 0;
		virtual bool IsColliding(const SphereCollider& collider, Vector3& mtv) const = 0;
		virtual bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const = 0;

		const Entity& entity;
		const Transform3& transform;

	private:
		Collider() = default;
	};

	class SphereCollider :
		public Collider
	{
	public:
		SphereCollider(const Entity& e, const Transform3& t, const Sphere& g) : Collider(e, t), geometry(g) {}
		friend CapsuleCollider;
		friend void Collide(
			const std::vector<Entity>& entities,
			const Components& components,
			std::vector<HitPair>& collisions);

	protected:
		const Sphere& geometry;

		inline bool IsColliding(const SphereCollider& collider) const final;
		inline bool IsColliding(const CapsuleCollider& collider) const final;
		inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const final;
		inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const final;

	private:
		SphereCollider() = default;
	};

	class CapsuleCollider :
		public Collider
	{
	public:
		CapsuleCollider(const Entity& e, const Transform3& t, const Capsule& g) : Collider(e, t), geometry(g) {}
		friend SphereCollider;
		friend void Collide(
			const std::vector<Entity>& entities,
			const Components& components,
			std::vector<HitPair>& collisions);

	protected:
		const Capsule& geometry;

		inline bool IsColliding(const CapsuleCollider& collider) const final;
		inline bool IsColliding(const SphereCollider& collider) const final;
		inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const final;
		inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const final;

	private:
		CapsuleCollider() = default;
	};

	inline bool SphereCollider::IsColliding(const SphereCollider& collider) const
	{
		return SphereSphere(collider.transform, transform, collider.geometry, geometry);
	}

	inline bool SphereCollider::IsColliding(const CapsuleCollider& collider) const
	{
		return SphereCapsule(transform, collider.transform, geometry, collider.geometry);
	}

	inline bool SphereCollider::IsColliding(const SphereCollider& collider, Vector3& mtv) const
	{
		return SphereSphere(collider.transform, transform, collider.geometry, geometry, mtv);
	}

	inline bool SphereCollider::IsColliding(const CapsuleCollider& collider, Vector3& mtv) const
	{
		bool isColliding = SphereCapsule(transform, collider.transform, geometry, collider.geometry, mtv);
		mtv = -mtv;
		return isColliding;
	}

	inline bool CapsuleCollider::IsColliding(const CapsuleCollider& collider) const
	{
		return CapsuleCapsule(collider.transform, transform, collider.geometry, geometry);
	}

	inline bool CapsuleCollider::IsColliding(const SphereCollider& collider) const
	{
		return SphereCapsule(collider.transform, transform, collider.geometry, geometry);
	}

	inline bool CapsuleCollider::IsColliding(const CapsuleCollider& collider, Vector3& mtv) const
	{
		return CapsuleCapsule(collider.transform, transform, collider.geometry, geometry, mtv);
	}

	inline bool CapsuleCollider::IsColliding(const SphereCollider& collider, Vector3& mtv) const
	{
		return SphereCapsule(collider.transform, transform, collider.geometry, geometry, mtv);
	}

	/*inline void Collide(
		const std::vector<SphereCollider>& spheres,
		const std::vector<CapsuleCollider>& capsules,
		std::vector<HitPair>& collisions)
	{
		std::vector<const SphereCollider*> staticSpheres;
		std::vector<const SphereCollider*> dynamicSpheres;
		std::vector<const CapsuleCollider*> staticCapsules;
		std::vector<const CapsuleCollider*> dynamicCapsules;

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
		}
	}*/

	/*
	enum Geometry
	{
		SPHERE,
		CAPSULE
	};

	virtual Geometry Type() const = 0;
	Geometry Type() const final { return Geometry::SPHERE; }
	Geometry Type() const final { return Geometry::CAPSULE; }
	*/

	/*for (Collider* collider : colliders)
	{
		switch (collider->Type())
		{
		case Geometry::SPHERE:
			SphereCollider* sphereCollider = static_cast<SphereCollider*>(collider);
			if (collider->dynamic)
				dynamicSpheres.push_back(sphereCollider);
			else
				staticSpheres.push_back(sphereCollider);
			break;

		case Geometry::CAPSULE:
			CapsuleCollider* capsuleCollider = static_cast<CapsuleCollider*>(collider);
			if (collider->dynamic)
				dynamicCapsules.push_back(capsuleCollider);
			else
				staticCapsules.push_back(capsuleCollider);
			break;
		}
	}*/

	/*
	struct StaticSphere { Id n; };
	struct StaticCapsule { Id n; };
	struct DynamicSphere { Id n; };
	struct DynamicCapsule { Id n; };

	class Colliders
	{
	public:
		UnorderedVector<SphereCollider> mStaticSpheres;
		UnorderedVector<SphereCollider> mDynamicSpheres;
		UnorderedVector<CapsuleCollider> mStaticCapsules;
		UnorderedVector<CapsuleCollider> mDynamicCapsules;

	public:
		void Add(StaticSphere& id, float radius,
			Transform3* transform = nullptr, Tags::Tag tag = Tags::NONE, void* data = nullptr)
		{
			SphereCollider collider(radius, transform, tag, data);
			id.n = mStaticSpheres.Add(collider);
			//id.n = mStaticSpheres.Add(std::move(collider));
		}

		void Add(DynamicSphere& id, float radius,
			Transform3* transform = nullptr, Tags::Tag tag = Tags::NONE, void* data = nullptr)
		{
			SphereCollider collider(radius, transform, tag, data);
			id.n = mDynamicSpheres.Add(collider);
			//id.n = mDynamicSpheres.Add(std::move(collider));
		}

		void Add(StaticCapsule& id, float halfHeight, float radius,
			Transform3* transform = nullptr, Tags::Tag tag = Tags::NONE, void* data = nullptr)
		{
			CapsuleCollider collider(halfHeight, radius, transform, tag, data);
			id.n = mStaticCapsules.Add(collider);
			//id.n = mStaticCapsules.Add(std::move(collider));
		}

		void Add(DynamicCapsule& id, float halfHeight, float radius,
			Transform3* transform = nullptr, Tags::Tag tag = Tags::NONE, void* data = nullptr)
		{
			CapsuleCollider collider(halfHeight, radius, transform, tag, data);
			id.n = mDynamicCapsules.Add(collider);
			//id.n = mDynamicCapsules.Add(std::move(collider));
		}

		SphereCollider* Get(StaticSphere id) { return mStaticSpheres.Get(id.n); }
		SphereCollider* Get(DynamicSphere id) { return mDynamicSpheres.Get(id.n); }
		CapsuleCollider* Get(StaticCapsule id) { return mStaticCapsules.Get(id.n); }
		CapsuleCollider* Get(DynamicCapsule id) { return mDynamicCapsules.Get(id.n); }

		void Remove(StaticSphere id) { mStaticSpheres.Remove(id.n); }
		void Remove(StaticCapsule id) { mStaticCapsules.Remove(id.n); }
		void Remove(DynamicSphere id) { mDynamicSpheres.Remove(id.n); };
		void Remove(DynamicCapsule id) { mDynamicCapsules.Remove(id.n); }

		void Collide(std::vector<HitPair>& collisions)
		{
			std::vector<SphereCollider>& staticSpheres = mStaticSpheres.Objects();
			std::vector<SphereCollider>& dynamicSpheres = mDynamicSpheres.Objects();
			std::vector<CapsuleCollider>& staticCapsules = mStaticCapsules.Objects();
			std::vector<CapsuleCollider>& dynamicCapsules = mDynamicCapsules.Objects();

			// Static spheres vs dynamic spheres & dynamic capsules
			for (int i = 0; i < staticSpheres.size(); i++)
			{
				for (int j = 0; j < dynamicSpheres.size(); j++)
				{
					Vector3 mtv;
					SphereCollider& a = staticSpheres[i];
					SphereCollider& b = dynamicSpheres[j];
					if (b.IsColliding(a, mtv))
					{
						collisions.push_back({ a.mInfo, b.mInfo, mtv });
					}
				}

				for (int j = 0; j < dynamicCapsules.size(); j++)
				{
					Vector3 mtv;
					SphereCollider& a = staticSpheres[i];
					CapsuleCollider& b = dynamicCapsules[j];
					if (b.IsColliding(a, mtv))
					{
						collisions.push_back({ a.mInfo, b.mInfo, mtv });
					}
				}
			}

			// Static capsules vs dynamic spheres & dynamic capsules
			for (int i = 0; i < staticCapsules.size(); i++)
			{
				for (int j = 0; j < dynamicSpheres.size(); j++)
				{
					Vector3 mtv;
					CapsuleCollider& a = staticCapsules[i];
					SphereCollider& b = dynamicSpheres[j];
					if (b.IsColliding(a, mtv))
					{
						collisions.push_back({ a.mInfo, b.mInfo, mtv });
					}
				}

				for (int j = 0; j < dynamicCapsules.size(); j++)
				{
					Vector3 mtv;
					CapsuleCollider& a = staticCapsules[i];
					CapsuleCollider& b = dynamicCapsules[j];
					if (b.IsColliding(a, mtv))
					{
						collisions.push_back({ a.mInfo, b.mInfo, mtv });
					}
				}
			}

			// Dynamic spheres vs dynamic spheres & dynamic capsules
			for (int i = 0; i < dynamicSpheres.size(); i++)
			{
				for (int j = 0; j < dynamicCapsules.size(); j++)
				{
					Vector3 mtv;
					SphereCollider& a = dynamicSpheres[i];
					CapsuleCollider& b = dynamicCapsules[j];
					if (b.IsColliding(a, mtv))
					{
						collisions.push_back({ a.mInfo, b.mInfo, mtv });
					}
				}

				for (int j = 0; j < dynamicSpheres.size(); j++)
				{
					if (i == j) continue;
					Vector3 mtv;
					SphereCollider& a = dynamicSpheres[i];
					SphereCollider& b = dynamicSpheres[j];
					if (b.IsColliding(a, mtv))
					{
						collisions.push_back({ a.mInfo, b.mInfo, mtv });
					}
				}
			}

			// Dynamic capsules vs dynamic capsules
			for (int i = 0; i < dynamicCapsules.size(); i++)
			{
				for (int j = 0; j < dynamicCapsules.size(); j++)
				{
					if (i == j) continue;
					Vector3 mtv;
					CapsuleCollider& a = dynamicCapsules[i];
					CapsuleCollider& b = dynamicCapsules[j];
					if (b.IsColliding(a, mtv))
					{
						collisions.push_back({ a.mInfo, b.mInfo, mtv });
					}
				}
			}
		}
	};*/
}
