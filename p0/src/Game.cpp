#include "pch.h"
#include "Game.h"
#include "Utility.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    mStepTimer.SetFixedTimeStep(true);
    mStepTimer.SetTargetElapsedSeconds(1.0 / 50.0);
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
    Mouse::Get().SetWindow(window);
    //Mouse::Get().SetMode(Mouse::MODE_RELATIVE);

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(mDeviceResources->GetD3DDevice(), mDeviceResources->GetD3DDeviceContext());
    ImGui::StyleColorsDark();

    Scene::Run();
}

// Main runs this function as frequently as possible -- whenever the Win32 event queue is empty
void Game::Tick()
{
    mStepTimer.Tick([&] { Scene::Update((float)mStepTimer.GetElapsedSeconds(), (float)mStepTimer.GetTotalSeconds()); });
    Input();
    Audio();
    Render();
}

void Game::Input()
{
    GamePad::State padState = GamePad::Get().GetState(0);
    if (padState.IsConnected())
    {
        if (padState.IsViewPressed())
        {
            ExitGame();
        }
    }

    Keyboard::State keyState = Keyboard::Get().GetState();
    if (keyState.Escape)
    {
        ExitGame();
    }
}

void Game::Audio()
{
    // IsCriticalError() = !IsAudioDevicePresent() == Update().
    // Not worth handling audio connect & disconnect at this point.
    mAudioEngine->Update();
}

void Game::Render()
{
    // Don't try to render anything before the two ticks (needed for frame interpolation/extrapolation).
    if (mStepTimer.GetFrameCount() <= 1)
    {
        return;
    }

    Clear();

    Scene::Render(mDeviceResources);
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Show the new frame.
    mDeviceResources->Present();
}

void Game::GUI()
{
}

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
    mStepTimer.ResetElapsedTime();
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
    Scene::Create(mDeviceResources, mAudioEngine, mScene);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    Scene::Resize(mDeviceResources);
}

void Game::OnDeviceLost()
{
    mScene = Scene::Scene::Current();
    Scene::Destroy();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
    Scene::Run();
}
#pragma endregion

// Gameloop timer test results:
// Debug: Tick() & Frame() fired 50:50
// Release: Tick() & Frame() fired 10:50

// Test code:
//static int yes, no; <-- ++yes within Tick(), ++no within Frame().
//static double timer = 0.0;
//timer += mStepTimer.GetElapsedSeconds();
//if (timer > 1.0)
//{
//    timer = 0.0;
//    Print("Yes: " + std::to_string(yes));
//    Print("No : " + std::to_string(no));
//    yes = 0;
//    no = 0;
//}
