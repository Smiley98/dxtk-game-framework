#include "pch.h"
#include "Game.h"
#include "scenes/Scene.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace scene;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_shared<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
    if (m_audEngine)
    {
        m_audEngine->Suspend();
    }
}

// Input and audio should be initialized before
// CreateDeviceDependentResources() and CreateWindowSizeDependentResources()
// since Scene expects them to be initialized by then.
void Game::Initialize(HWND window, int width, int height)
{
    m_gamePad = std::make_shared<GamePad>();
    m_keyboard = std::make_shared<Keyboard>();
    m_mouse = std::make_shared<Mouse>();
    m_mouse->SetWindow(window);

    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags |= AudioEngine_Debug;
#endif
    m_audEngine = std::make_shared<AudioEngine>(eflags);

    m_deviceResources->SetWindow(window, width, height);
    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();
    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    Scene::Run(m_scene);
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    if (!m_audEngine->IsCriticalError())
        m_audEngine->Update();

    Render();
}

// Updates the world.
void Game::Update(const DX::StepTimer& timer)
{
    auto const pad = m_gamePad->GetState(0);
    if (pad.IsConnected())
    {
        if (pad.IsViewPressed())
        {
            ExitGame();
        }
    }

    auto const kb = m_keyboard->GetState();
    if (kb.Escape)
    {
        ExitGame();
    }

    Scene::Update(timer, *m_gamePad, *m_keyboard, *m_mouse);
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    Scene::Render(m_deviceResources);
    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
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
    m_audEngine->Suspend();
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();
    m_audEngine->Resume();
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
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
    Scene::Create(m_deviceResources, m_audEngine);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // RECT r = m_deviceResources->GetOutputSize(); // Use this internally if you want screen size information
    Scene::Resize(m_deviceResources);
}

void Game::OnDeviceLost()
{
    m_scene = Scene::Current();
    Scene::Destroy();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
    Scene::Run(m_scene);
}
#pragma endregion
