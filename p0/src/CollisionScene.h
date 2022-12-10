#pragma once
#include "Scene.h"

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
    void OnUpdate(float dt, float tt, const DX::Input& input) final;
    void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	struct TestSphereSphere
	{
		Vector3 a;
		Vector3 b;
		Color color;
	} mSS;	// sphere-sphere

	struct TestCapsuleCapsule
	{
		Transform a;
		Transform b;
		Color color;
	} mCC;	// capsule-capsule

	struct TestSphereCapsule
	{
		Vector3 a;
		Transform b;
		Color color;
	} mSC;	// sphere-capsule (resolves capsule)

	struct TestSoccer
	{
		Transform player;
		Vector3 ball;
		Color color;
	} mSoccer;	// capsule-sphere (resolves sphere)

	struct TestRange
	{
		Transform viewer;
		Vector3 target;
		float fov = 45.0f;
		float length = 100.0f;
	} mRange;	// FoV-test
};
