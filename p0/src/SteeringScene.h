#pragma once
#include "Scene.h"
class SteeringScene :
    public Scene
{
public:
    SteeringScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
    ~SteeringScene() final;

   void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
   void OnBegin() final;
   void OnEnd() final;
   void OnPause() final;
   void OnResume() final;
   void OnUpdate(float dt, float tt, const DX::Input& input) final;
   void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
    Entity mSeeker, mArriver;
    Entity mAvoidingSeeker, mSensor;
    Entity mWanderer, mRandomTarget;
    Entity mAvoider1, mAvoider2;

    std::vector<Entity> mMap;

    void Reset();
};

