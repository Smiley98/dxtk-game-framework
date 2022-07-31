#pragma once
#include "DeviceResources.h"
#include "StepTimer.h"
#include "SceneType.h"

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
    void Update(DX::StepTimer const& timer);
    void Render();
    void Clear();
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Shared resources 
    std::shared_ptr<DX::DeviceResources>    mDeviceResources;
    std::shared_ptr<DirectX::AudioEngine>   mAudioEngine;
    std::shared_ptr<DirectX::GamePad>       mGamePad;
    std::shared_ptr<DirectX::Keyboard>      mKeyboard;
    std::shared_ptr<DirectX::Mouse>         mMouse;
    DX::StepTimer                           mTimer;
    Scene::Type                             mScene = Scene::MAIN;
};