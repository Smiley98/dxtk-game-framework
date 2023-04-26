#pragma once
#include "Scene.h"
#include "Curves.h"
#include "Geometry.h"

class SplineScene :
    public Scene
{
public:
    SplineScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
    ~SplineScene() final;

    void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
    void OnBegin() final;
    void OnEnd() final;
    void OnPause() final;
    void OnResume() final;
    void OnUpdate(float dt, float tt) final;
    void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
    Spline mSpline;
    Entity mSplineFollower;
    std::vector<Vector3> mSplineRender;

    std::vector<Entity> mRacers;
    std::vector<Entity> mCheckpoints;
    std::vector<Line> mLines;

    void CreateRacer(size_t index);
    void FollowTrack(Entity& entity);
};
