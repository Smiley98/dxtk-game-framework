#include "pch.h"
#include "TestScene.h"

#include "DeviceResources.h"
#include "StepTimer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace scene
{
	TestScene::TestScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
	{
		m_waveBank = std::make_unique<WaveBank>(audio.get(), L"assets/sounds/adpcmdroid.xwb");
		m_soundEffect = std::make_unique<SoundEffect>(audio.get(), L"assets/music/MusicMono_adpcm.wav");
		m_effect1 = m_soundEffect->CreateInstance();
		m_effect2 = m_waveBank->CreateInstance(10);

		auto context = graphics->GetD3DDeviceContext();
		auto device = graphics->GetD3DDevice();

		m_states = std::make_unique<CommonStates>(device);
		m_fxFactory = std::make_unique<EffectFactory>(device);
		m_sprites = std::make_unique<SpriteBatch>(context);
		m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
		m_batchEffect = std::make_unique<BasicEffect>(device);
		m_batchEffect->SetVertexColorEnabled(true);

		DX::ThrowIfFailed(
			CreateInputLayoutFromEffect<VertexPositionColor>(device,
				m_batchEffect.get(),
				m_batchInputLayout.ReleaseAndGetAddressOf())
		);

		m_font = std::make_unique<SpriteFont>(device, L"assets/fonts/SegoeUI_18.spritefont");
		m_shape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

		// SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
		m_fxFactory->SetDirectory(L"assets/models");
		m_model = Model::CreateFromSDKMESH(device, L"assets/models/tiny.sdkmesh", *m_fxFactory);

		// Load textures
		DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/seafloor.dds", nullptr, m_texture1.ReleaseAndGetAddressOf()));
		DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/windowslogo.dds", nullptr, m_texture2.ReleaseAndGetAddressOf()));
	}

	TestScene::~TestScene()
	{
		m_states.reset();
		m_fxFactory.reset();
		m_sprites.reset();
		m_batch.reset();
		m_batchEffect.reset();
		m_font.reset();
		m_shape.reset();
		m_model.reset();
		m_texture1.Reset();
		m_texture2.Reset();
		m_batchInputLayout.Reset();
	}

	void TestScene::OnResize(int width, int height, std::shared_ptr<DX::DeviceResources> graphics)
	{
		const RECT size = graphics->GetOutputSize();
		const float aspectRatio = float(size.right) / float(size.bottom);
		float fovAngleY = 70.0f * XM_PI / 180.0f;

		// This is a simple example of change that can be made when the app is in
		// portrait or snapped view.
		if (aspectRatio < 1.0f)
		{
			fovAngleY *= 2.0f;
		}

		// This sample makes use of a right-handed coordinate system using row-major matrices.
		m_projection = Matrix::CreatePerspectiveFieldOfView(
			fovAngleY,
			aspectRatio,
			0.01f,
			100.0f
		);

		m_batchEffect->SetProjection(m_projection);
	}

	void TestScene::OnBegin()
	{
		m_effect1->Play(true);
		m_effect2->Play();
	}

	void TestScene::OnEnd()
	{
	}

	void TestScene::OnPause()
	{
	}

	void TestScene::OnResume()
	{
	}

	void TestScene::OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse)
	{
		const Vector3 eye(0.0f, 0.7f, 1.5f);
		const Vector3 at(0.0f, -0.1f, 0.0f);
		m_view = Matrix::CreateLookAt(eye, at, Vector3::UnitY);
		m_world = Matrix::CreateRotationY(float(timer.GetTotalSeconds() * XM_PIDIV4));
		m_batchEffect->SetView(m_view);
		m_batchEffect->SetWorld(Matrix::Identity);
	}

	void TestScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
	{
		graphics->PIXBeginEvent(L"Render");
		auto context = graphics->GetD3DDeviceContext();

		// Draw procedurally generated dynamic grid
		const XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
		const XMVECTORF32 yaxis = { 0.f, 0.f, 20.f };
		DrawGrid(graphics, xaxis, yaxis, g_XMZero, 20, 20, Colors::Gray);
		//DrawGrid(xaxis, yaxis, g_XMZero, 20, 20, Colors::Gray);

		// Draw sprite
		graphics->PIXBeginEvent(L"Draw sprite");
		m_sprites->Begin();
		m_sprites->Draw(m_texture2.Get(), XMFLOAT2(10, 75), nullptr, Colors::White);

		m_font->DrawString(m_sprites.get(), L"DirectXTK Simple Sample", XMFLOAT2(100, 10), Colors::Yellow);
		m_sprites->End();
		graphics->PIXEndEvent();

		// Draw 3D object
		graphics->PIXBeginEvent(L"Draw teapot");
		XMMATRIX local = m_world * Matrix::CreateTranslation(-2.f, -2.f, -4.f);
		m_shape->Draw(local, m_view, m_projection, Colors::White, m_texture1.Get());
		graphics->PIXEndEvent();

		graphics->PIXBeginEvent(L"Draw model");
		const XMVECTORF32 scale = { 0.01f, 0.01f, 0.01f };
		const XMVECTORF32 translate = { 3.f, -2.f, -4.f };
		const XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(XM_PI / 2.f, 0.f, -XM_PI / 2.f);
		local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);
		m_model->Draw(context, *m_states, local, m_view, m_projection);
		graphics->PIXEndEvent();

		graphics->PIXEndEvent();
	}

	void XM_CALLCONV TestScene::DrawGrid(std::shared_ptr<DX::DeviceResources> graphics, DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color)
	{
		graphics->PIXBeginEvent(L"Draw grid");

		auto context = graphics->GetD3DDeviceContext();
		context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
		context->OMSetDepthStencilState(m_states->DepthNone(), 0);
		context->RSSetState(m_states->CullCounterClockwise());

		m_batchEffect->Apply(context);

		context->IASetInputLayout(m_batchInputLayout.Get());

		m_batch->Begin();

		xdivs = std::max<size_t>(1, xdivs);
		ydivs = std::max<size_t>(1, ydivs);

		for (size_t i = 0; i <= xdivs; ++i)
		{
			float fPercent = float(i) / float(xdivs);
			fPercent = (fPercent * 2.0f) - 1.0f;
			XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
			vScale = XMVectorAdd(vScale, origin);

			const VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
			const VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
			m_batch->DrawLine(v1, v2);
		}

		for (size_t i = 0; i <= ydivs; i++)
		{
			float fPercent = float(i) / float(ydivs);
			fPercent = (fPercent * 2.0f) - 1.0f;
			XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
			vScale = XMVectorAdd(vScale, origin);

			const VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
			const VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
			m_batch->DrawLine(v1, v2);
		}

		m_batch->End();

		graphics->PIXEndEvent();
	}
}
