#pragma once
#include "Scene.h"

class CollisionScene :
    public Scene
{
public:
    CollisionScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components);
    ~CollisionScene() final;

    void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
    void OnBegin() final;
    void OnEnd() final;
    void OnPause() final;
    void OnResume() final;
    void OnUpdate(float dt, float tt, const DX::Input& input) final;
    void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	struct TestSphereSphere
	{
		Entity a;
		Entity b;
		Color color;
	} mSS;	// sphere-sphere

	struct TestCapsuleCapsule
	{
		Entity a;
		Entity b;
		Color color;
	} mCC;	// capsule-capsule

	struct TestSphereCapsule
	{
		Entity a;
		Entity b;
		Color color;
	} mSC;	// sphere-capsule (resolves capsule)

	struct TestSoccer
	{
		Entity player;
		Entity ball;
		Color color;
	} mSoccer;	// capsule-sphere (resolves sphere)

	struct TestRange
	{
		Entity viewer;
		Entity target;
		float fov = 45.0f;
		float length = 100.0f;
	} mRange;	// FoV-test
};
