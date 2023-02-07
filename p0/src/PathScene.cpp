#include "pch.h"
#include "PathScene.h"
#include "DebugRenderer.h"

using namespace DirectX;

PathScene::PathScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	: Scene(graphics, audio)
{
}

PathScene::~PathScene()
{
}

void PathScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 60.0f * XM_RADIANS;
	fovAngleY = aspectRatio < 1.0f ? fovAngleY * 2.0f : fovAngleY;
	float halfWidth = mWorldWidth * 0.5f;
	float halfHeight = mWorldHeight * 0.5f;
	mView = Matrix::CreateLookAt({ halfWidth, halfHeight, 1000.0f }, { halfWidth, halfHeight, 0.0f }, Vector3::Up);
	mProj = Matrix::CreateOrthographic(mWorldWidth, mWorldHeight, 0.1f, 10000.0f);
}

void PathScene::OnBegin()
{
}

void PathScene::OnEnd()
{
}

void PathScene::OnPause()
{
}

void PathScene::OnResume()
{
}

void PathScene::OnUpdate(float dt, float tt)
{	
}

void PathScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	Debug::DrawBox({ 800.0f, 450.0f, 0.0f }, { 160.0f, 90.0f, 1.0f });
	sPlayerRenderer.Render(sComponents.GetTransform(sPlayer).World(), mView, mProj, graphics);
}
