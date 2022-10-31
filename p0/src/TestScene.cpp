#include "pch.h"
#include "TestScene.h"
#include "Utility.h"

using namespace DirectX;

TestScene::TestScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
{
	//mWaveBank = std::make_unique<WaveBank>(audio.get(), L"assets/sounds/adpcmdroid.xwb");
	//mSoundEffect = std::make_unique<SoundEffect>(audio.get(), L"assets/music/MusicMono_adpcm.wav");
	//mEffect1 = mSoundEffect->CreateInstance();
	//mEffect2 = mWaveBank->CreateInstance(10);

	auto context = graphics->GetD3DDeviceContext();
	auto device = graphics->GetD3DDevice();

	mStates = std::make_unique<CommonStates>(device);
	mFxFactory = std::make_unique<EffectFactory>(device);
	mSprites = std::make_unique<SpriteBatch>(context);
	mBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
	mBatchEffect = std::make_unique<BasicEffect>(device);
	mBatchEffect->SetVertexColorEnabled(true);

	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(device,
			mBatchEffect.get(),
			mBatchInputLayout.ReleaseAndGetAddressOf())
	);

	mFont = std::make_unique<SpriteFont>(device, L"assets/fonts/SegoeUI_18.spritefont");
	mShape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

	// SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
	mFxFactory->SetDirectory(L"assets/models");
	mModel = Model::CreateFromSDKMESH(device, L"assets/models/tiny.sdkmesh", *mFxFactory);

	// Load textures
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/seafloor.dds", nullptr, mTexture1.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/windowslogo.dds", nullptr, mTexture2.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/van.dds", nullptr, mTextureVan.ReleaseAndGetAddressOf()));
	
	mVanEffect = std::make_shared<BasicEffect>(device);
	mVanEffect->SetTextureEnabled(true);
	mVanEffect->SetTexture(mTextureVan.Get());
	
	mVan = Model::CreateFromVBO(device, L"assets/meshes/vanZ.vbo", mVanEffect);
}

TestScene::~TestScene()
{
	// Pointers get automatically reset since they're intellectuals ;)
}

void TestScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
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
	mProj = Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		mNear,
		mFar
	);

	mBatchEffect->SetProjection(mProj);
}

void TestScene::OnBegin()
{
	//mEffect1->Play(true);
	//mEffect2->Play();

	//mTransform.Orientate(Quaternion::CreateFromYawPitchRoll(M_PI_4, M_PI_4, 0.0f));
	//Print(mTransform.Forward());
	//Print(mTransform.Rotation());

	//AddTimer("test", 1.0f, [this]() {
	//	Print(mTransform.Forward());
	//	Print(mTransform.Rotation());
	//}, true);

	mParent.SetName("Parent");
	mChildA.SetName("Child A");
	mChildB.SetName("Child B");

	mParent.AddChild(&mChildA);
	mParent.AddChild(&mChildB);
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

void TestScene::OnUpdate(float dt, float tt, DX::Input& input)
{
	mView = Matrix::CreateLookAt({ 0.0f, 500.0f, mNear }, Vector3::Zero, Vector3::Up);
	mWorld = Matrix::CreateRotationY(tt * XM_PIDIV4);
	
	mTransform.RotateY(tt * 50.0f);
	mTransform.DeltaTranslate(mTransform.Forward() * dt * 100.0f);

	mBatchEffect->SetView(mView);
	mBatchEffect->SetWorld(Matrix::Identity);
}

void TestScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	graphics->PIXBeginEvent(L"Render");
	auto context = graphics->GetD3DDeviceContext();

	// Draw procedurally generated dynamic grid
	const XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
	const XMVECTORF32 yaxis = { 0.f, 0.f, 20.f };
	DrawGrid(graphics, xaxis, yaxis, g_XMZero, 20, 20, Colors::Gray);

	// Draw sprite
	graphics->PIXBeginEvent(L"Draw sprite");
	mSprites->Begin();
	mSprites->Draw(mTexture2.Get(), XMFLOAT2(10, 75), nullptr, Colors::White);

	// Draw text
	mFont->DrawString(mSprites.get(), L"DirectXTK Simple Sample", XMFLOAT2(100, 10), Colors::Yellow);
	mSprites->End();
	graphics->PIXEndEvent();

	// Draw 3D objects

	//XMMATRIX local = mWorld * Matrix::CreateTranslation(0.0f, -2.0f, -4.0f);
	//{	graphics->PIXBeginEvent(L"Draw teapot");
	//	mTransform.Translate({ 0.0f, -2.0f, -4.0f });
	//	mShape->Draw(mTransform.LocalMatrix(), mView, mProj, Colors::White, mTexture1.Get());
	//}	graphics->PIXEndEvent();
	
	//{	graphics->PIXBeginEvent(L"Draw sdkmesh");
	//	const XMVECTORF32 scale = { 0.01f, 0.01f, 0.01f };
	//	const XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(XM_PI / 2.f, 0.f, -XM_PI / 2.f);
	//	const XMVECTORF32 translate = { 3.f, -2.f, -4.f };
	//	XMMATRIX local = mWorld * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);
	//	mModel->Draw(context, *mStates, local, mView, mProj);
	//}	graphics->PIXEndEvent();

	//const Vector3 scale(0.1f);
	//const XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(XM_PI / 2.f, 0.f, -XM_PI / 2.f);
	//const XMVECTORF32 translate = { 2.f, -1.f, -4.f };
	//XMMATRIX local = mWorld * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);
	{	graphics->PIXBeginEvent(L"Draw vbo (van)");
		mVan->Draw(context, *mStates, mTransform.LocalMatrix(), mView, mProj);
	}	graphics->PIXEndEvent();

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
