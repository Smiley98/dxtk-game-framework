#pragma once
#include "Scene.h"

class Game final : public DX::IDeviceNotify
{
public:
    Game() noexcept(false);
    ~Game();

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() final;
    void OnDeviceRestored() final;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);
    void NewAudioDevice();

    void GetDefaultSize( int& width, int& height ) const noexcept;
    
private:
    void Input();
    void Audio();
    void Render();
    void Clear();
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Shared resources
    DX::StepTimer mStepTimer;
    std::shared_ptr<DX::DeviceResources> mDeviceResources;
    std::shared_ptr<DirectX::AudioEngine> mAudioEngine;
    DX::Input mInput;

    Components mComponents;
    Scene::Type mScene = Scene::ENTITY;
};
