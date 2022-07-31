#include "pch.h"
#include "MainScene.h"
#include "DebugRenderer.h"
#include "Collision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace Scene
{
	MainScene::MainScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
	{
		auto context = graphics->GetD3DDeviceContext();
		auto device = graphics->GetD3DDevice();

		// TODO: Make a Buildings class that loads all building vbos
		// Even though buildings only exist in the MainScene,
		// its still more convenient to contain them within a class
		mTd = Model::CreateFromVBO(device, L"assets/meshes/td.vbo", sBuildingShader);

		mStates = std::make_unique<CommonStates>(device);

		mVanTransform.Rotate(-45.0f);
		mVanTransform.Translate(mVanTransform.Front() * -750.0f);

		Vector3 scale = sVan->meshes.front()->boundingBox.Extents * 2.0f;

		mCylinderTransform.Scale(scale);
		mCylinderTransform.Translate(-100.0f, 0.0f);

		mBoxTransform.Scale(scale);
		mBoxTransform.Translate(100.0f, 0.0f);
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
		const float dt = (float)timer.GetElapsedSeconds();
		const float tt = (float)timer.GetTotalSeconds();

		mView = Matrix::CreateLookAt({ 0.0f, -100.0f, 1000.0f }, {}, Vector3::UnitY);
		mVanTransform.DeltaRotate(cosf(tt) * 0.4f);
		mVanTransform.DeltaTranslate(mVanTransform.Front() * dt * 100.0f);
	}

	void MainScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
	{
		auto context = graphics->GetD3DDeviceContext();


		sVan->Draw(context, *mStates, mVanTransform.World(), mView, mProjection);
		mTd->Draw(context, *mStates, mTdTransform.World(), mView, mProjection);

		Debug::Primitive(Debug::BOX, mBoxTransform.World(), mView, mProjection, graphics);
		Debug::Primitive(Debug::CYLINDER, mCylinderTransform.World(), mView, mProjection, graphics);
	}
}
