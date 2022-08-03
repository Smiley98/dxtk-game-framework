#include "pch.h"
#include "PhysicsScene.h"
#include "DebugRenderer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PhysicsScene::PhysicsScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
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

	// Van as capsule
	//mVanCollider = { {}, vanBounds.y, vanBounds.x };
	//mVanCollider.transform.Rotate({ 0.0f, 0.0f, -45.0f });
	//mVanCollider.transform.Translate(mVanCollider.transform.Front() * -750.0f);

	// Van as sphere
	mVanCollider.radius = vanBounds.x;
	mVanCollider.translation = { -500.0f, -500.0f, 0.0f };

	// TD as sphere
	//mTdCollider.radius = (tdBounds.x + tdBounds.y) * 0.5f;

	// TD as capsule
	mTdCollider.radius = tdBounds.x;
	mTdCollider.halfHeight = tdBounds.y;

	//mSphereCollider.radius = (tdBounds.x + tdBounds.y) * 0.5f;

	//mCapsuleCollider.transform.Translate({ tdBounds.x, tdBounds.y, 0.0f });
	//mCapsuleCollider.transform.Rotate({ 0.0f, 0.0f, 90.0f });
	//mCapsuleCollider.radius = tdBounds.x;
	//mCapsuleCollider.halfHeight = tdBounds.y;
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 60.0f * XM_PI / 180.0f;
	fovAngleY = aspectRatio < 1.0f ? fovAngleY * 2.0f : fovAngleY;
	mProjection = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.01f, 10000.0f);
}

void PhysicsScene::OnBegin()
{
}

void PhysicsScene::OnEnd()
{
}

void PhysicsScene::OnPause()
{
}

void PhysicsScene::OnResume()
{
}

void PhysicsScene::OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse)
{
	mView = Matrix::CreateLookAt({ 0.0f, -100.0f, 1000.0f }, {}, Vector3::UnitY);
	const float dt = (float)timer.GetElapsedSeconds();
	const float tt = (float)timer.GetTotalSeconds();
	float speed = 100.0f * dt;

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

	// Capsule-Sphere test
	//mVanCollider.transform.DeltaTranslate({ speed, speed, 0.0f });
	//mVanCollider.transform.Rotate({ 0.0f, 0.0f, cosf(tt) * 100.0f });
	//Vector3 mtv;
	//if (mVanCollider.IsColliding(mTdCollider, mtv))
	//	mVanCollider.transform.DeltaTranslate(mtv);
	//mColor = mVanCollider.IsColliding(mTdCollider, mtv) ? Colors::Red : Colors::Green;

	// Questionable van soccer
	//mVanCollider.transform.DeltaRotate({ 0.0f, 0.0f, cosf(tt) * 0.4f });
	//mVanCollider.transform.DeltaTranslate(mVanCollider.transform.Front() * speed);
	mVanCollider.translation += {speed, speed, 0.0f};
	Vector3 mtv;
	if (mTdCollider.IsColliding(mVanCollider, mtv))
		mTdCollider.transform.Translate(mtv);
	//mTdCollider.translation = mtv;
	mColor = mTdCollider.IsColliding(mVanCollider, mtv) ? Colors::Red : Colors::Green;
}

void PhysicsScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto context = graphics->GetD3DDeviceContext();

	//Debug::Draw(mSphereCollider, mView, mProjection, graphics, mSphereColor);
	//Debug::Draw(mCapsuleCollider, mView, mProjection, graphics, mCapsuleColor);

	//mTd->Draw(context, *mStates, Matrix::Identity, mView, mProjection);
	//sVan->Draw(context, *mStates, mVanCollider.transform.World(), mView, mProjection);

	Debug::Draw(mVanCollider, mView, mProjection, graphics, mColor);
	Debug::Draw(mTdCollider, mView, mProjection, graphics);
}
