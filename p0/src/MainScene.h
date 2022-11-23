#pragma once
#include "Scene.h"

class MainScene :
	public Scene
{
public:
	MainScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
	~MainScene() final;

protected:
	virtual void OnResize(std::shared_ptr<DX::DeviceResources> graphics) override;

	virtual void OnBegin() override;
	virtual void OnEnd() override;

	virtual void OnPause() override;
	virtual void OnResume() override;

	virtual void OnUpdate(float dt, float tt, DX::Input& input) override;
	virtual void OnRender(std::shared_ptr<DX::DeviceResources> graphics) override;

private:
	
};
