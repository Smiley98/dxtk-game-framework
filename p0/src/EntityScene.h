#pragma once
#include "Scene.h"
#include "Collision.h"
#include "Map.h"
#include <array>

struct Player
{
	Collision::DynamicCapsule id;
	Objects::Player mType = Objects::VAN;
	RigidTransform* transform;

	void Load(const PlayerRenderer& renderer, Collision::Colliders& colliders)
	{
		Vector3 bounds = renderer.Bounds(mType);
		colliders.Add(id, bounds.y - bounds.x, bounds.x, Tags::PLAYER, this);
		transform = colliders.Get(id);
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
	Collision::Colliders mColliders;
	std::array<Collision::StaticCapsule, 2> mStatic;
	std::array<Collision::DynamicCapsule, 2> mDynamic;
	Map mMap;
	Player mVan;
	BuildingId mBuilding;
	Vector4 mColor;
};
