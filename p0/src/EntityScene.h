#pragma once
#include "Scene.h"
#include "Collision.h"
#include "Map.h"
#include "Curves.h"

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
	Building mBuilding;

	Transform mHeadlights;
	const float fov = 60.0f;
	const float range = 400.0f;

	float mWorldWidth = 1600;
	float mWorldHeight = 900;

	std::vector<Vector3> mSpline = {
		Vector3{ 500.0f, -300.0f, 0.0f },
		Vector3{ -500.0f, -300.0f, 100.0f },
		Vector3{ -500.0f, 300.0f, 100.0f },
		Vector3{ 500.0f, 300.0f, 0.0f }
	};
	SpeedTable mSpeedTable;

	float t = 0.0f, d = 0.0f;
	size_t interval = 0, sample = 0;
};
