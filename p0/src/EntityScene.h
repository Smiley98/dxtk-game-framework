#pragma once
#include "Scene.h"
#include "Map.h"
#include "Curves.h"

class EntityScene :
	public Scene
{
public:
	EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components);
	~EntityScene() final;

	void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
	void OnBegin() final;
	void OnEnd() final;
	void OnPause() final;
	void OnResume() final;
	void OnUpdate(float dt, float tt, const DX::Input& input) final;
	void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	std::vector<Entity> mBuildings;
	std::array<Entity, Building::COUNT> mTestBuildings;

	Entity mPlayer;
	Entity mHeadlights;

	const float fov = 60.0f;
	const float range = 400.0f;

	float mWorldWidth = 1600;
	float mWorldHeight = 900;

	std::vector<Vector3> mSpline = {
		Vector3{ 500.0f, -300.0f, -500.0f },
		Vector3{ -500.0f, -300.0f, 500.0f },
		Vector3{ -500.0f, 300.0f, 500.0f },
		Vector3{ 500.0f, 300.0f, -500.0f }
	};
	SpeedTable mSpeedTable;

	float t = 0.0f, d = 0.0f;
	size_t interval = 0, sample = 0;

	Entity mSeeker, mArriver, mRandomSeeker, mRandomTarget;
	Entity mAvoider1, mAvoider2, mTarget1, mTarget2;
	Entity mParent, mChild1, mChild2;
};
