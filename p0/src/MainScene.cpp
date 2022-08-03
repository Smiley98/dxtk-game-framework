#include "pch.h"
#include "MainScene.h"
#include "DebugRenderer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MainScene::MainScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	auto context = graphics->GetD3DDeviceContext();
	auto device = graphics->GetD3DDevice();
	mStates = std::make_unique<CommonStates>(device);

	// TODO: Make a Buildings class that loads all building vbos
	// Even though buildings only exist in the MainScene,
	// its still more convenient to contain them within a class
	mBuildingShader = std::make_shared<BasicEffect>(device);
	mBuildingShader->EnableDefaultLighting();
	mBuildingShader->SetLightDirection(0, sLightDirection);
	mBuildingShader->SetAmbientLightColor(sAmbient);
	mBuildingShader->SetDiffuseColor(sDiffuse);
	mBuildingShader->SetSpecularColor(sSpecular);
	mBuildingShader->SetSpecularPower(1024.0f);
	mBuildingShader->SetTextureEnabled(false);

	mTd = Model::CreateFromVBO(device, L"assets/meshes/td.vbo", mBuildingShader);
	Vector3 tdBounds = mTd->meshes.front()->boundingBox.Extents;
	Vector3 vanBounds = sVan->meshes.front()->boundingBox.Extents;
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
	mProjection = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.01f, 10000.0f);
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
