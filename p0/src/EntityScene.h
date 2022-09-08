#pragma once
#include "Scene.h"
#include "Collision.h"
#include "Map.h"
#include <array>

struct Player
{
	Collision::DynamicCapsule id;
	Objects::Player type = Objects::VAN;
	RigidTransform* transform;

	void Load(const PlayerRenderer& renderer, Collision::Colliders& colliders)
	{
		Vector3 bounds = renderer.Bounds(type);
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
	void OnUpdate(float dt, float tt, DX::Input& input) final;
	void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	Collision::Colliders mColliders;
	MintyAftershave mMap;
	Player mVan;

	struct Camera
	{
		Transform transform;
		float angle = 0.0f;
		const float fov = 60.0f;
		const float range = 400.0f;
	} mCamera;

	float mWorldWidth = 1600;
	float mWorldHeight = 900;
};
