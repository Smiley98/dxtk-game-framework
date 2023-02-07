#pragma once
#include "Scene.h"
#include "TileMap.h"

class PathScene : public Scene
{
public:
	PathScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
	~PathScene() final;

	void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
	void OnBegin() final;
	void OnEnd() final;
	void OnPause() final;
	void OnResume() final;

	void OnUpdate(float dt, float tt) final;
	void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	Pathing::TileMap mMap;
};
