#include "pch.h"
#include "TestScene.h"
#include "Utility.h"

using namespace DirectX;

TestScene::TestScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components)
	: Scene(graphics, audio, components)
{
	//mWaveBank = std::make_unique<WaveBank>(audio.get(), L"assets/sounds/adpcmdroid.xwb");
	//mSoundEffect = std::make_unique<SoundEffect>(audio.get(), L"assets/music/MusicMono_adpcm.wav");
	//mEffect1 = mSoundEffect->CreateInstance();
	//mEffect2 = mWaveBank->CreateInstance(10);

	auto context = graphics->GetD3DDeviceContext();
	auto device = graphics->GetD3DDevice();

	mStates = std::make_unique<CommonStates>(device);
	//mFxFactory = std::make_unique<EffectFactory>(device);
	//mSprites = std::make_unique<SpriteBatch>(context);
	mBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
	mBatchEffect = std::make_unique<BasicEffect>(device);
	mBatchEffect->SetVertexColorEnabled(true);

	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(device,
			mBatchEffect.get(),
			mBatchInputLayout.ReleaseAndGetAddressOf())
	);

	//mFont = std::make_unique<SpriteFont>(device, L"assets/fonts/SegoeUI_18.spritefont");
	mShape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

	// SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
	//mFxFactory->SetDirectory(L"assets/models");
	//mModel = Model::CreateFromSDKMESH(device, L"assets/models/tiny.sdkmesh", *mFxFactory);

	// Load textures
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/seafloor.dds", nullptr, mTexture1.ReleaseAndGetAddressOf()));
	//DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/windowslogo.dds", nullptr, mTexture2.ReleaseAndGetAddressOf()));
}

TestScene::~TestScene()
{
	// Pointers get automatically reset since they're intellectuals ;)
}

void TestScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 70.0f * XM_RADIANS;
	float n = 0.001f, f = 1000.0f;
	mView = Matrix::CreateLookAt({ 0.0f, 50.0f, n }, Vector3::Zero, Vector3::Up);
	mProj = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, n, f);
	mBatchEffect->SetProjection(mProj);
	mBatchEffect->SetView(mView);
	mBatchEffect->SetWorld(Matrix::Identity);
}

void TestScene::OnBegin()
{
	//mEffect1->Play(true);
	//mEffect2->Play();
	//AddTimer("test", 1.0f, [this]() {}, true);

	// Note that moving along forward vectors will translate "upwards" since the engine uses
	// Y_FORWARD but this scene is Z_FORWARD.
	mParent = CreateEntity(mComponents);
	mChild1 = CreateEntity(mComponents);
	mChild2 = CreateEntity(mComponents);
	AddChild(mParent, mChild1, mComponents);
	AddChild(mParent, mChild2, mComponents);

	EntityTransform& parent = *mComponents.transforms.GetComponent(mParent);
	EntityTransform& child1 = *mComponents.transforms.GetComponent(mChild1);
	EntityTransform& child2 = *mComponents.transforms.GetComponent(mChild2);

	child1.Scale(2.0f);
	child2.Scale(2.0f);
	child1.RotateY(-30.0f);
	child2.RotateY(30.0f);
	parent.Translate(0.0f, 0.0f, 10.0f);
	child1.Translate(-5.0f, 0.0f, 10.0f);
	child2.Translate(5.0f, 0.0f, 10.0f);
}

void TestScene::OnEnd()
{
	//mEffect1->Stop();
	//mEffect2->Stop();
}

void TestScene::OnPause()
{
}

void TestScene::OnResume()
{
}

void TestScene::OnUpdate(float dt, float tt, const DX::Input& input)
{
	mComponents.transforms.GetComponent(mParent)->RotateY(tt * 50.0f);
	mComponents.transforms.GetComponent(mChild1)->RotateX(tt * 50.0f);
	mComponents.transforms.GetComponent(mChild2)->RotateZ(tt * 50.0f);
}

void TestScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	graphics->PIXBeginEvent(L"Render");
	auto context = graphics->GetD3DDeviceContext();

	const XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
	const XMVECTORF32 yaxis = { 0.f, 0.f, 20.f };
	DrawGrid(graphics, xaxis, yaxis, g_XMZero, 20, 20, Colors::Gray);

	//graphics->PIXBeginEvent(L"Draw sprite");
	//mSprites->Begin();
	//mSprites->Draw(mTexture2.Get(), XMFLOAT2(10, 75), nullptr, Colors::White);
	//
	//mFont->DrawString(mSprites.get(), L"DirectXTK Simple Sample", XMFLOAT2(100, 10), Colors::Yellow);
	//mSprites->End();
	//graphics->PIXEndEvent();

	mShape->Draw(mComponents.transforms.GetComponent(mParent)->World(), mView, mProj, Colors::White, mTexture1.Get());
	mShape->Draw(mComponents.transforms.GetComponent(mChild1)->World(), mView, mProj, Colors::White, mTexture1.Get());
	mShape->Draw(mComponents.transforms.GetComponent(mChild2)->World(), mView, mProj, Colors::White, mTexture1.Get());

	graphics->PIXEndEvent();
}

void XM_CALLCONV TestScene::DrawGrid(std::shared_ptr<DX::DeviceResources> graphics, DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color)
{
	graphics->PIXBeginEvent(L"Draw grid");

	auto context = graphics->GetD3DDeviceContext();
	context->OMSetBlendState(mStates->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(mStates->DepthNone(), 0);
	context->RSSetState(mStates->CullCounterClockwise());

	mBatchEffect->Apply(context);

	context->IASetInputLayout(mBatchInputLayout.Get());

	mBatch->Begin();

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
		mBatch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= ydivs; i++)
	{
		float fPercent = float(i) / float(ydivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		const VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
		const VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
		mBatch->DrawLine(v1, v2);
	}

	mBatch->End();

	graphics->PIXEndEvent();
}

// LHS seems more intuitive than RHS, but the depth buffer is flipped so gotta re-export models with LHS...
//mView = XMMatrixLookAtRH({ 0.0f, 50.0f, 50.0f }, Vector3::Zero, Vector3::Up);
//mProj = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, near, far);
//mView = XMMatrixLookAtLH({ 0.0f, 50.0f, 50.0f }, Vector3::Zero, Vector3::Up);
//mProj = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, n, f);
