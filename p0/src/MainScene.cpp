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

	mVanTransform.Rotate({ 0.0f, 0.0f, -45.0f });
	mVanTransform.Translate(mVanTransform.Front() * -750.0f);
	mVanCollider = { mVanTransform, vanBounds.y, vanBounds.x };

	// TD as sphere -- max bounds for real values, 50.0 for testing
	//mTdCollider.radius = std::max(tdBounds.x, tdBounds.y);
	//mTdCollider.radius = 50.0f;
	//mTdCollider.translation = Vector3::Zero;
	
	// TD as capsule
	mTdCollider.radius = tdBounds.x;
	mTdCollider.halfHeight = tdBounds.y;

	//mSphereCollider.radius = std::max(tdBounds.x, tdBounds.y);
	//mSphereCollider.radius = 50.0f;
	
	mCapsuleCollider.transform.Rotate({ 0.0f, 0.0f, 90.0f });
	mCapsuleCollider.radius = vanBounds.x;
	mCapsuleCollider.halfHeight = vanBounds.y;
	mCapsuleColor = Colors::Green;
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
	mVanTransform.DeltaRotate({0.0f, 0.0f, cosf(tt) * 0.4f });
	mVanTransform.DeltaTranslate(mVanTransform.Front() * dt * 100.0f);
	mVanCollider.transform = mVanTransform;

	//mSphereCollider.translation = { mSphereCollider.radius * cos(tt), mSphereCollider.radius * sin(tt), 0.0f };
	//Vector3 mtv;
	//if (mSphereCollider.IsColliding(mTdCollider, mtv))
	//	mSphereCollider.translation += mtv;
	//mSphereColor = mSphereCollider.IsColliding(mTdCollider, mtv) ? Colors::Red : Colors::Green;

	//mCapsuleCollider.translation = { mCapsuleCollider.radius * cos(tt), mSphereCollider.radius * sin(tt), 0.0f };
	//Vector3 mtv;
	//if (mCapsuleCollider.IsColliding(mTdCollider, mtv))
	//	mCapsuleCollider.transform.DeltaTranslate(mtv);
	//mCapsuleColor = mCapsuleCollider.IsColliding(mTdCollider, mtv) ? Colors::Red : Colors::Green;
}

void MainScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	//SphereCollider sphereColliderLeft{ { mVanTransform.Translation() + -3.0f * mVanExtents.x * mVanTransform.Adjacent() }, mVanExtents.x};
	//SphereCollider sphereColliderRight{ { mVanTransform.Translation() + 3.0f * mVanExtents.x * mVanTransform.Adjacent() }, mVanExtents.x};
	//Debug::Draw(sphereColliderLeft, mView, mProjection, graphics);
	//Debug::Draw(sphereColliderRight, mView, mProjection, graphics);

	//mTd->Draw(context, *mStates, Matrix::Identity, mView, mProjection);
	//sVan->Draw(context, *mStates, mVanTransform.World(), mView, mProjection);
	//Debug::Draw(mVanCollider, mView, mProjection, graphics);

	//Debug::Draw(mSphereCollider, mView, mProjection, graphics, mSphereColor);
	Debug::Draw(mCapsuleCollider, mView, mProjection, graphics, mCapsuleColor);

	Debug::Draw(mTdCollider, mView, mProjection, graphics);
}
