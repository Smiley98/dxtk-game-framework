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
    void OnUpdate(float dt, float tt, DX::Input& input) final;
    void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	struct TestSphereSphere
	{
		Transform3 tA;
		Transform3 tB;
		Sphere gA;
		Sphere gB;
		Color color;
	} mSS;	// sphere-sphere

	struct TestCapsuleCapsule
	{
		Transform3 tA;
		Transform3 tB;
		Capsule gA;
		Capsule gB;
		Color color;
	} mCC;	// capsule-capsule

	struct TestSphereCapsule
	{
		Transform3 tA;
		Transform3 tB;
		Sphere gA;
		Capsule gB;
		Color color;
	} mSC;	// sphere-capsule (resolves capsule)

	struct TestSoccer
	{
		Transform3 tPlayer;
		Transform3 tBall;
		Capsule gPlayer;
		Sphere gBall;
		Color color;
	} mSoccer;	// capsule-sphere (resolves sphere)
};
