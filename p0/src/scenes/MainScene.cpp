#include "pch.h"
#include "MainScene.h"
#include "DeviceResources.h"
#include "StepTimer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace scene
{
	MainScene::MainScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
	{
		auto context = graphics->GetD3DDeviceContext();
		auto device = graphics->GetD3DDevice();

		mStates = std::make_unique<CommonStates>(device);
		mShader = std::make_shared<BasicEffect>(device);

		mShader->SetLightingEnabled(true);
		mShader->SetPerPixelLighting(true);
		mShader->SetLightDirection(0, Vector3::UnitZ * -1.0f);

		mShader->SetAmbientLightColor(Colors::Red);
		mShader->SetDiffuseColor(Colors::Green);
		mShader->SetSpecularColor(Colors::Blue);
		mShader->SetSpecularPower(32.0f);

		//mShader->SetTextureEnabled(true);
		//mShader->SetTexture(m_texture.Get());
		//DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/van.dds", nullptr, mTexture.ReleaseAndGetAddressOf()));
		//mVbo = Model::CreateFromVBO(device, L"assets/meshes/td.vbo", mShader);	// Downtown Toronto was exported correctly ;)
		mVbo = Model::CreateFromVBO(device, L"assets/meshes/van.vbo", mShader);

		mTransform.Scale(25.0f);
		mTransform.Rotate(-45.0f);
		mTransform.Translate(mTransform.Front() * -750.0f);
	}

	MainScene::~MainScene()
	{
		mStates.reset();
		mVbo.reset();
		mShader.reset();
		//mTexture.Reset();
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
		const float dt = (float)timer.GetElapsedSeconds();
		const float tt = (float)timer.GetTotalSeconds();

		mView = Matrix::CreateLookAt({ 0.0f, -100.0f, 1000.0f }, {}, Vector3::UnitY);
		mTransform.DeltaRotate(cosf(tt) * 0.4f);
		mTransform.DeltaTranslate(mTransform.Front() * dt * 100.0f);
	}

	void MainScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
	{
		auto context = graphics->GetD3DDeviceContext();
		
		mVbo->Draw(context, *mStates, mTransform.World(), mView, mProjection);
	}
}
