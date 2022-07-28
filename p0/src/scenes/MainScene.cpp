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

		m_states = std::make_unique<CommonStates>(device);
		m_shader = std::make_shared<BasicEffect>(device);

		m_shader->SetLightingEnabled(true);
		m_shader->SetPerPixelLighting(true);
		m_shader->SetLightDirection(0, Vector3::UnitY);

		m_shader->SetAmbientLightColor(Colors::Red);
		m_shader->SetDiffuseColor(Colors::Green);
		m_shader->SetSpecularColor(Colors::Blue);
		m_shader->SetSpecularPower(32.0f);

		//m_shader->SetTextureEnabled(true);
		//m_shader->SetTexture(m_texture.Get());
		//DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/van.dds", nullptr, m_texture.ReleaseAndGetAddressOf()));
		m_vbo = Model::CreateFromVBO(device, L"assets/meshes/cup.vbo", m_shader);
	}

	MainScene::~MainScene()
	{
		m_states.reset();
		m_vbo.reset();
		m_shader.reset();
		//m_texture.Reset();
	}

	void MainScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
	{
		const RECT size = graphics->GetOutputSize();
		const float aspectRatio = float(size.right) / float(size.bottom);
		float fovAngleY = 35.0f * XM_PI / 180.0f;
		fovAngleY = aspectRatio < 1.0f ? fovAngleY * 2.0f : fovAngleY;
		m_projection = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.01f, 100.0f);
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
		// eye { 0.0f, 0.0f, -10.0f } up UnitY for front view, eye { 0.0f, -10.0f, 0.0f } up UnitZ for top view
		const Vector3 eye(0.0f, -10.0f, 0.0f);
		const Vector3 at(0.0f, 0.0f, 0.0f);
		m_view = Matrix::CreateLookAt(eye, at, Vector3::UnitZ);
		m_world = Matrix::CreateRotationY(float(timer.GetTotalSeconds() * XM_PIDIV4));
	}

	void MainScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
	{
		auto context = graphics->GetD3DDeviceContext();

		XMMATRIX local = m_world * Matrix::CreateTranslation(1.0f, 0.0f, 1.0f);
		m_vbo->Draw(context, *m_states, local, m_view, m_projection);
	}
}
