#pragma once
#include "Scene.h"
#include "Collision.h"

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

	virtual void OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse) override;
	virtual void OnRender(std::shared_ptr<DX::DeviceResources> graphics) override;

private:
	std::unique_ptr<DirectX::CommonStates> mStates;
	std::shared_ptr<DirectX::BasicEffect> mBuildingShader;

	SphereCollider mSphereCollider;
	DirectX::XMVECTOR mSphereColor;

	CapsuleCollider mCapsuleCollider;
	DirectX::XMVECTOR mCapsuleColor;

	std::unique_ptr<DirectX::Model>	mTd;
	CapsuleCollider mTdCollider;
	//SphereCollider mTdCollider;

	CapsuleCollider mVanCollider;
	Transform mVanTransform;

	DirectX::SimpleMath::Matrix	mView;
	DirectX::SimpleMath::Matrix	mProjection;
};
