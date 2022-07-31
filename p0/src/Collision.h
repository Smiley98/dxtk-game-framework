#pragma once
#include "Tags.h"
#include "Transform.h"

#include <vector>
namespace collision
{
// TODO: static vs dynamic (building spheres vs everything else -- may not need all 4 vectors)
	//static void AddStatic(const SphereCollider& collider);
	//static void AddStatic(const CapsuleCollider& collider);
	//static void AddDynamic(const SphereCollider& collider);
	//static void AddDynamic(const CapsuleCollider& collider);
	//static void RemoveStatic(const SphereCollider& collider);
	//static void RemoveStatic(const CapsuleCollider& collider);
	//static void RemoveDynamic(const SphereCollider& collider);
	//static void RemoveDynamic(const CapsuleCollider& collider);

	//std::vector<SphereCollider> sStaticSpheres;
	//std::vector<SphereCollider> sDynamicSpheres;
	//std::vector<CapsuleCollider> sStaticCapsules;
	//std::vector<CapsuleCollider> sDynamicCapsules;

	struct HitPair
	{
		Tag tag1;
		Tag tag2;
		void* data1;
		void* data2;
		DirectX::SimpleMath::Vector3 mtv;
	};

	class SphereCollider;
	class CapsuleCollider;
	class Collider
	{
	public:
		Collider() = default;
		~Collider() = default;
		Collider(Tag tag, void* data = nullptr);
		uint32_t Id() const;

		// These shouldn't exist. Any necessary references should be stored on-add and released on-remove.
		//static const SphereCollider* GetSphere(uint32_t id);
		//static const CapsuleCollider* GetCapsule(uint32_t id);

		static SphereCollider& Add(const DirectX::SimpleMath::Vector3& translation, float radius,
			Tag tag, void* data = nullptr);
		static CapsuleCollider& Add(const DirectX::SimpleMath::Vector3& translation, const DirectX::SimpleMath::Vector3& orientation, float halfHeight, float radius,
			Tag tag, void* data = nullptr);

		static void Remove(const SphereCollider& collider);
		static void Remove(const CapsuleCollider& collider);

		static void Clear();
		static std::vector<HitPair> Collisions();

		virtual void Update(const Transform& transform) = 0;

	protected:
		virtual bool Collision(const SphereCollider& collider, DirectX::SimpleMath::Vector3& mtv) = 0;
		virtual bool Collision(const CapsuleCollider& collider, DirectX::SimpleMath::Vector3& mtv) = 0;

	private:
		static std::vector<SphereCollider> sSpheres;
		static std::vector<CapsuleCollider> sCapsules;
		static uint32_t sId;

		// No need to store members related to static/dynamic, or capsule/sphere
		// because we already know that based on data structures (no Collider* nonsense).
		uint32_t mId = 0;
		Tag mTag = NONE;
		void* mData = nullptr;
	};

	class SphereCollider :
		public Collider
	{
		friend class DebugRenderer;
	public:
		SphereCollider() = default;
		~SphereCollider() = default;
		SphereCollider(const DirectX::SimpleMath::Vector3& translation, float radius,
			Tag tag, void* data = nullptr);

		void Update(const Transform& transform) final;

	protected:
		bool Collision(const SphereCollider& collider, DirectX::SimpleMath::Vector3& mtv) final;
		bool Collision(const CapsuleCollider& collider, DirectX::SimpleMath::Vector3& mtv) final;

	private:
		DirectX::SimpleMath::Vector3 mTranslation;
		float mRadius = 0.0f;
	};

	class CapsuleCollider :
		public Collider
	{
		friend class DebugRenderer;
	public:
		CapsuleCollider() = default;
		~CapsuleCollider() = default;
		CapsuleCollider(const DirectX::SimpleMath::Vector3& translation, const DirectX::SimpleMath::Vector3& orientation, float halfHeight, float radius,
			Tag tag, void* data = nullptr);

		void Update(const Transform& transform) final;

	protected:
		bool Collision(const SphereCollider& collider, DirectX::SimpleMath::Vector3& mtv) final;
		bool Collision(const CapsuleCollider& collider, DirectX::SimpleMath::Vector3& mtv) final;

	private:
		DirectX::SimpleMath::Vector3 mTranslation;
		DirectX::SimpleMath::Vector3 mOrientation;
		float mHalfHeight = 0.0f;
		float mRadius = 0.0f;
	};
}