#pragma once
#include "Scene.h"
#include "Collision.h"

// Placeholder for now. In the future this will be data-oriented rather than monolithic xD
struct Entity
{
	uint32_t colliderId;
	Transform transform;
};

struct Player : public Entity
{
	Objects::Player mType = Objects::VAN;

	void Load(const PlayerRenderer& renderer, Collision& collision)
	{
		Vector3 bounds = renderer.Bounds(mType);

		Capsule capsule;
		capsule.r = bounds.x;
		capsule.hh = bounds.y - capsule.r;

		Collision::Info info;
		info.tag = Tags::Tag::PLAYER;
		info.data = this;
		
		colliderId = collision.mDynamicCapsules.Add(std::move(capsule), std::move(info));
	}

	void UpdateCollider(Collision& collision)
	{
		CapsuleCollider& c = collision.mDynamicCapsules.Get(colliderId);
		c.g.t = transform;
	}
};

struct Building : public Entity
{
	Objects::Building mType = Objects::TD;

	void Load(const BuildingRenderer& renderer, Collision& collision)
	{
		Sphere sphere;
		sphere.r = renderer.Bounds(mType).x;

		Collision::Info info;
		info.tag = Tags::Tag::BUILDING;
		info.data = this;

		colliderId = collision.mStaticSpheres.Add(std::move(sphere), std::move(info));
	}

	void UpdateCollider(Collision& collision)
	{
		SphereCollider& c = collision.mStaticSpheres.Get(colliderId);
		c.g.t = transform.Translation();
	}
};

class EntityScene :
	public Scene
{
public:
	EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
	~EntityScene() final;

	void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
	void OnBegin() final;
	void OnEnd() final;
	void OnPause() final;
	void OnResume() final;
	void OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse) final;
	void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	Collision mCollision;
	DirectX::XMVECTOR mColor;

	Player mVan;
	Building mTd;

	//Collision::Id ssId = mCollision.AddStatic({ Vector3::Zero, 0.0f }, {});
	//Collision::Id dsId = mCollision.AddDynamic({ Vector3::Zero, 0.0f }, {});
	//Collision::Id scId = mCollision.AddStatic({ {}, 0.0f, 0.0f }, {});
	//Collision::Id dcId = mCollision.AddDynamic({ {}, 0.0f, 0.0f }, {});
	//mCollision.Remove(ssId);
	//mCollision.Remove(dsId);
	//mCollision.Remove(scId);
	//mCollision.Remove(dcId);
	//printf("Success!\n");
};

