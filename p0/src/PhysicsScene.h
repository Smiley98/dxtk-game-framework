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

	const float mRadius = 25.0f;
	const float mHalfHeight = 75.0f;

	SphereCollider mSphereSphereA;
	SphereCollider mSphereSphereB;
	DirectX::XMVECTOR mSphereSphereColor;

	CapsuleCollider mCapsuleCapsuleA;
	CapsuleCollider mCapsuleCapsuleB;
	DirectX::XMVECTOR mCapsuleCapsuleColor;

	void DebugSpheres(const Sphere& a, const Sphere& b, std::shared_ptr<DX::DeviceResources> graphics);
	void DebugCapsules(const Capsule& a, const Capsule& b, std::shared_ptr<DX::DeviceResources> graphics);

	//SphereCollider mSphereCapsuleA;
	//CapsuleCollider mSphereCapsuleB;
	//DirectX::XMVECTOR mSphereCapsuleColor;
	//
	//CapsuleCollider mCapsuleSphereA;
	//SphereCollider mCapsuleSphereB;
	//DirectX::XMVECTOR mCapsuleSphereColor;
};
