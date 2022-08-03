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
	DirectX::SimpleMath::Matrix	mView;
	DirectX::SimpleMath::Matrix	mProjection;

	const float mRadius = 25.0f;
	const float mHalfHeight = 75.0f;

	SphereCollider mSphereSphereA;
	SphereCollider mSphereSphereB;
	DirectX::XMVECTOR mSphereSphereColor;

	CapsuleCollider mCapsuleCapsuleA;
	CapsuleCollider mCapsuleCapsuleB;
	DirectX::XMVECTOR mCapsuleCapsuleColor;

	//SphereCollider mSphereCapsuleA;
	//CapsuleCollider mSphereCapsuleB;
	//DirectX::XMVECTOR mSphereCapsuleColor;
	//
	//CapsuleCollider mCapsuleSphereA;
	//SphereCollider mCapsuleSphereB;
	//DirectX::XMVECTOR mCapsuleSphereColor;
};

