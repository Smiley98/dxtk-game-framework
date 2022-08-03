#pragma once
#include "Scene.h"
#include "Collision.h"

class PhysicsScene :
    public Scene
{
public:
    PhysicsScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
    ~PhysicsScene() final;

    void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
    void OnBegin() final;
    void OnEnd() final;
    void OnPause() final;
    void OnResume() final;
    void OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse) final;
    void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	std::unique_ptr<DirectX::CommonStates> mStates;
	std::shared_ptr<DirectX::BasicEffect> mBuildingShader;

	//SphereCollider mSphereCollider;
	//DirectX::XMVECTOR mSphereColor;
	//CapsuleCollider mCapsuleCollider;
	//DirectX::XMVECTOR mCapsuleColor;

	std::unique_ptr<DirectX::Model>	mTd;
	CapsuleCollider mTdCollider;
	//SphereCollider mTdCollider;

	//CapsuleCollider mVanCollider;
	SphereCollider mVanCollider;

	DirectX::XMVECTOR mColor;

	DirectX::SimpleMath::Matrix	mView;
	DirectX::SimpleMath::Matrix	mProjection;
};

