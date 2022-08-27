#pragma once
#include "Scene.h"
#include "Collision.h"

class CollisionScene :
    public Scene
{
public:
    CollisionScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
    ~CollisionScene() final;

    void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
    void OnBegin() final;
    void OnEnd() final;
    void OnPause() final;
    void OnResume() final;
    void OnUpdate(const DX::StepTimer& timer, DX::Input& input) final;
    void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	Collision::SphereCollider mSphereSphereA;
	Collision::SphereCollider mSphereSphereB;
	DirectX::XMVECTOR mSphereSphereColor;

	Collision::CapsuleCollider mCapsuleCapsuleA;
	Collision::CapsuleCollider mCapsuleCapsuleB;
	DirectX::XMVECTOR mCapsuleCapsuleColor;

	Collision::SphereCollider mSphere1;
	Collision::CapsuleCollider mCapsule1;
	DirectX::XMVECTOR mColor1;

	Collision::SphereCollider mSphere2;
	Collision::CapsuleCollider mCapsule2;
	DirectX::XMVECTOR mColor2;

	void DebugSpheres(const Collision::SphereCollider& a, const Collision::SphereCollider& b, std::shared_ptr<DX::DeviceResources> graphics);
	void DebugCapsules(const Collision::CapsuleCollider& a, const Collision::CapsuleCollider& b, std::shared_ptr<DX::DeviceResources> graphics);
};
