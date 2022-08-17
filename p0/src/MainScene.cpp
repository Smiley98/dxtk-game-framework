#include "pch.h"
#include "MainScene.h"
#include "DebugRenderer.h"

using namespace DirectX;

MainScene::MainScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	auto context = graphics->GetD3DDeviceContext();
	auto device = graphics->GetD3DDevice();
}

MainScene::~MainScene()
{
}

void MainScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 60.0f * XM_PI / 180.0f;
	fovAngleY = aspectRatio < 1.0f ? fovAngleY * 2.0f : fovAngleY;
	mProj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.01f, 10000.0f);
	//mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 100.0f }, {}, Vector3::UnitY);
	//mProj = Matrix::CreateOrthographic(width, height, 0.01f, 1000.0f);
}

void MainScene::OnBegin()
{
}

void MainScene::OnEnd()
{
}

void MainScene::OnPause()
{
}

void MainScene::OnResume()
{
}

void MainScene::OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse)
{
	mView = Matrix::CreateLookAt({ 0.0f, -100.0f, 1000.0f }, {}, Vector3::UnitY);
	const float dt = (float)timer.GetElapsedSeconds();
	const float tt = (float)timer.GetTotalSeconds();
}

void MainScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();
}
