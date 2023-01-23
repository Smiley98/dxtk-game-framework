#pragma once
#include "Scene.h"
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
	std::vector<Entity> mMap;
	std::array<Entity, Building::COUNT> mTestBuildings;
};
