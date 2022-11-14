#pragma once
#include "CollisionMath2.h"
#include "UnorderedVector.h"
#include "Tags.h"

namespace Collision2
{
	class Colliders;
	class SphereCollider;
	class CapsuleCollider;

	struct Info
	{
		Tags::Tag tag = Tags::NONE;	// Used to cast data
		void* data = nullptr;		// Points to the object that owns this collider
	};

	struct HitPair
	{
		Info a;
		Info b;
		Vector3 mtv;
	};

	class Collider
	{
	public:
		Collider() = default;
		Collider(Transform3* transformPtr, Tags::Tag tag, void* data) :
			transform(transformPtr), mInfo({ tag, data }) {}

		virtual bool IsColliding(const SphereCollider& collider) const = 0;
		virtual bool IsColliding(const CapsuleCollider& collider) const = 0;
		virtual bool IsColliding(const SphereCollider& collider, Vector3& mtv) const = 0;
		virtual bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const = 0;

		Transform3* transform = nullptr;

	protected:
		Info mInfo;
	};

	class SphereCollider :
		public Collider
	{
	public:
		SphereCollider() = default;
		SphereCollider(float radius,
			Transform3* transformPtr = nullptr, Tags::Tag tag = Tags::NONE, void* data = nullptr) :
			Collider(transformPtr, tag, data), mRadius(radius) {}

		float Radius() const { return mRadius; }

		inline bool IsColliding(const SphereCollider& collider) const final;
		inline bool IsColliding(const CapsuleCollider& collider) const final;
		inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const final;
		inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const final;

		friend Colliders;
	private:
		float mRadius;
	};

	class CapsuleCollider :
		public Collider
	{
	public:
		CapsuleCollider() = default;
		CapsuleCollider(float halfHeight, float radius,
			Transform3* transformPtr = nullptr, Tags::Tag tag = Tags::NONE, void* data = nullptr) :
			Collider(transformPtr, tag, data), mHalfHeight(halfHeight), mRadius(radius) {}

		float HalfHeight() const { return mHalfHeight; }
		float Radius() const { return mRadius; }

		inline bool IsColliding(const CapsuleCollider& collider) const final;
		inline bool IsColliding(const SphereCollider& collider) const final;
		inline bool IsColliding(const CapsuleCollider& collider, Vector3& mtv) const final;
		inline bool IsColliding(const SphereCollider& collider, Vector3& mtv) const final;

		friend Colliders;
	private:
		float mHalfHeight;
		float mRadius;
	};

	inline bool SphereCollider::IsColliding(const SphereCollider& collider) const
	{
		return SphereSphere(collider.transform->Translation(), transform->Translation(), collider.Radius(), Radius());
	}

	inline bool SphereCollider::IsColliding(const CapsuleCollider& collider) const
	{
		return SphereCapsule(transform->Translation(), Radius(), *collider.transform, collider.HalfHeight(), collider.Radius());
	}

	inline bool SphereCollider::IsColliding(const SphereCollider& collider, Vector3& mtv) const
	{
		return SphereSphere(collider.transform->Translation(), transform->Translation(), collider.Radius(), Radius(), mtv);
	}

	inline bool SphereCollider::IsColliding(const CapsuleCollider& collider, Vector3& mtv) const
	{
		bool isColliding = SphereCapsule(transform->Translation(), Radius(), *collider.transform, collider.HalfHeight(), collider.Radius(), mtv);
		mtv = -mtv;
		return isColliding;
	}

	inline bool CapsuleCollider::IsColliding(const CapsuleCollider& collider) const
	{
		return CapsuleCapsule(*collider.transform, *transform, collider.HalfHeight(), HalfHeight(), collider.Radius(), Radius());
	}

	inline bool CapsuleCollider::IsColliding(const SphereCollider& collider) const
	{
		return SphereCapsule(collider.transform->Translation(), collider.Radius(), *transform, HalfHeight(), Radius());
	}

	inline bool CapsuleCollider::IsColliding(const CapsuleCollider& collider, Vector3& mtv) const
	{
		return CapsuleCapsule(*collider.transform, *transform, collider.HalfHeight(), HalfHeight(), collider.Radius(), Radius(), mtv);
	}

	inline bool CapsuleCollider::IsColliding(const SphereCollider& collider, Vector3& mtv) const
	{
		return SphereCapsule(collider.transform->Translation(), collider.Radius(), *transform, HalfHeight(), Radius(), mtv);
	}

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
			id.n = mStaticSpheres.Add(std::move(collider));
		}

		void Add(DynamicSphere& id, float radius,
			Transform3* transform = nullptr, Tags::Tag tag = Tags::NONE, void* data = nullptr)
		{
			SphereCollider collider(radius, transform, tag, data);
			id.n = mDynamicSpheres.Add(std::move(collider));
		}

		void Add(StaticCapsule& id, float halfHeight, float radius,
			Transform3* transform = nullptr, Tags::Tag tag = Tags::NONE, void* data = nullptr)
		{
			CapsuleCollider collider(halfHeight, radius, transform, tag, data);
			id.n = mStaticCapsules.Add(std::move(collider));
		}

		void Add(DynamicCapsule& id, float halfHeight, float radius,
			Transform3* transform = nullptr, Tags::Tag tag = Tags::NONE, void* data = nullptr)
		{
			CapsuleCollider collider(halfHeight, radius, transform, tag, data);
			id.n = mDynamicCapsules.Add(std::move(collider));
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
	};
}
