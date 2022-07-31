#pragma once
#include "Tags.h"
#include "Transform.h"

#include <vector>
namespace Collision
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
		Collider(const Transform& transform, Tag tag, void* data = nullptr);
		inline uint32_t Id() const { return mId; }

		static SphereCollider& AddSphere(const Transform& transform, Tag tag, void* data = nullptr);
		static CapsuleCollider& AddCapsule(const Transform& transform, Tag tag, void* data = nullptr);

		static void Remove(const SphereCollider& collider);
		static void Remove(const CapsuleCollider& collider);

		static void Clear();
		static std::vector<HitPair> Collisions();

		inline operator Transform() const noexcept { return mTransform; }
		inline void SetTransform(const Transform& transform) { mTransform = transform; }

	protected:
		virtual bool Collision(const SphereCollider& collider, DirectX::SimpleMath::Vector3& mtv) = 0;
		virtual bool Collision(const CapsuleCollider& collider, DirectX::SimpleMath::Vector3& mtv) = 0;
		Transform mTransform;

	private:
		static std::vector<SphereCollider> sSpheres;
		static std::vector<CapsuleCollider> sCapsules;
		static uint32_t sId;

		Tag mTag = NONE;
		void* mData = nullptr;
		uint32_t mId = 0;
	};

	class SphereCollider :
		public Collider
	{
		friend class DebugRenderer;
	public:
		SphereCollider() = default;
		~SphereCollider() = default;
		SphereCollider(const Transform& transform, Tag tag, void* data = nullptr);

		inline DirectX::SimpleMath::Vector3 Position() const { return mTransform.Translation(); }
		inline float Radius() const { return mTransform.Scaling().Length(); }

	protected:
		bool Collision(const SphereCollider& collider, DirectX::SimpleMath::Vector3& mtv) final;
		bool Collision(const CapsuleCollider& collider, DirectX::SimpleMath::Vector3& mtv) final;

	private:
		// Instead of optimizing for space, decompose internal transform during collision test
		//DirectX::SimpleMath::Vector3 mTranslation;
		//float mRadius = 0.0f;
	};

	class CapsuleCollider :
		public Collider
	{
		friend class DebugRenderer;
	public:
		CapsuleCollider() = default;
		~CapsuleCollider() = default;
		CapsuleCollider(const Transform& transform, Tag tag, void* data = nullptr);

		inline DirectX::SimpleMath::Vector3 Translation() const { return mTransform.Translation(); }
		inline DirectX::SimpleMath::Vector3 Orientation() const { return mTransform.Front(); }
		inline float Radius()		const { return mTransform.Scaling().x; }
		inline float HalfHeight()	const { return mTransform.Scaling().y; }

	protected:
		bool Collision(const SphereCollider& collider, DirectX::SimpleMath::Vector3& mtv) final;
		bool Collision(const CapsuleCollider& collider, DirectX::SimpleMath::Vector3& mtv) final;

	private:
		// Instead of optimizing for space, decompose internal transform during collision test
		//DirectX::SimpleMath::Vector3 mTranslation;
		//DirectX::SimpleMath::Vector3 mOrientation;
		//float mHalfHeight = 0.0f;
		//float mRadius = 0.0f;
	};
}