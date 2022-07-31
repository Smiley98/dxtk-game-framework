#include "pch.h"
#include "Game.h"
#include "Scene.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    mDeviceResources = std::make_shared<DX::DeviceResources>();
    mDeviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
    if (mAudioEngine)
    {
        mAudioEngine->Suspend();
    }
}

// Input and audio should be initialized before
// CreateDeviceDependentResources() and CreateWindowSizeDependentResources()
// since Scene expects them to be initialized by then.
void Game::Initialize(HWND window, int width, int height)
{
    mGamePad = std::make_shared<GamePad>();
    mKeyboard = std::make_shared<Keyboard>();
    mMouse = std::make_shared<Mouse>();
    mMouse->SetWindow(window);

    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags |= AudioEngine_Debug;
#endif
    mAudioEngine = std::make_shared<AudioEngine>(eflags);

    mDeviceResources->SetWindow(window, width, height);
    mDeviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();
    mDeviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    Scene::Scene::Run(mScene);
}

// Executes the basic game loop.
void Game::Tick()
{
    mTimer.Tick([&]()
    {
        Update(mTimer);
    });

    if (!mAudioEngine->IsCriticalError())
        mAudioEngine->Update();

    Render();
}

// Updates the world.
void Game::Update(const DX::StepTimer& timer)
{
    auto const pad = mGamePad->GetState(0);
    if (pad.IsConnected())
    {
        if (pad.IsViewPressed())
        {
            ExitGame();
        }
    }

    auto const kb = mKeyboard->GetState();
    if (kb.Escape)
    {
        ExitGame();
    }

    Scene::Scene::Update(timer, *mGamePad, *mKeyboard, *mMouse);
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (mTimer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    Scene::Scene::Render(mDeviceResources);
    mDeviceResources->PIXEndEvent();

    // Show the new frame.
    mDeviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    mDeviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = mDeviceResources->GetD3DDeviceContext();
    auto renderTarget = mDeviceResources->GetRenderTargetView();
    auto depthStencil = mDeviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = mDeviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    mDeviceResources->PIXEndEvent();
}

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
    mAudioEngine->Suspend();
}

void Game::OnResuming()
{
    mTimer.ResetElapsedTime();
    mAudioEngine->Resume();
}

void Game::OnWindowMoved()
{
    auto const r = mDeviceResources->GetOutputSize();
    mDeviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    mDeviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!mDeviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();
}

void Game::NewAudioDevice()
{
}

void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    Scene::Scene::Create(mDeviceResources, mAudioEngine);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // RECT r = m_deviceResources->GetOutputSize(); // Use this internally if you want screen size information
    Scene::Scene::Resize(mDeviceResources);
}

void Game::OnDeviceLost()
{
    mScene = Scene::Scene::Current();
    Scene::Scene::Destroy();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
    Scene::Scene::Run(mScene);
}
#pragma endregion
