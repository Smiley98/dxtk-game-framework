#include "pch.h"
#include "SplineScene.h"
#include "EntityFunctions.h"
#include "Utility.h"

using namespace DirectX;

SplineScene::SplineScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components)
	: Scene(graphics, audio, components)
{
	mHeadlights = CreateEntity(mComponents);
	mComponents.transforms.GetComponent(mHeadlights)->TranslateY(80.0f);
	mComponents.transforms.GetComponent(mHeadlights)->Scale(100.0f);
	AddChild(sPlayer, mHeadlights, mComponents);

	mSpline.points = {
		Vector3{ 500.0f, -300.0f, -500.0f },
		Vector3{ -500.0f, -300.0f, 500.0f },
		Vector3{ -500.0f, 300.0f, 500.0f },
		Vector3{ 500.0f, 300.0f, -500.0f }
	};
	mSpline.speedTable = CreateSpeedTable(mSpline.points, 16);
}

SplineScene::~SplineScene()
{
}

void SplineScene::OnResize(std::shared_ptr<DX::DeviceResources> graphics)
{
	const RECT size = graphics->GetOutputSize();
	const float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = 60.0f * XM_RADIANS;
	fovAngleY = aspectRatio < 1.0f ? fovAngleY * 2.0f : fovAngleY;
	mView = Matrix::CreateLookAt({ 0.0f, 0.0f, 1000.0f }, Vector3::Zero, Vector3::Up);
	mProj = Matrix::CreateOrthographic(mWorldWidth, mWorldHeight, 0.1f, 10000.0f);
}

void SplineScene::OnBegin()
{
}

void SplineScene::OnEnd()
{
}

void SplineScene::OnPause()
{
}

void SplineScene::OnResume()
{
}

void SplineScene::OnUpdate(float dt, float tt, const DX::Input& input)
{
	FollowPath(dt, 250.0f, mSpline, sPlayer, mComponents);

	Vector3 position = mComponents.transforms.GetComponent(sPlayer)->WorldPosition();
	mNearest = NearestProjection(position, mSpline.points);
}

void SplineScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	for (const Vector3& position : mSpline.points)
		Debug::Sphere(position, 50.0f, mView, mProj, graphics);

	for (size_t i = 1; i <= mSpline.points.size(); i++)
		Debug::Line(mSpline.points[i - 1], mSpline.points[i % mSpline.points.size()], 10.0f,
			mView, mProj, graphics);

	Debug::Sphere(mNearest, 50.0f, mView, mProj, graphics);
	sPlayerRenderer.Render(mComponents.transforms.GetComponent(sPlayer)->World(), mView, mProj, graphics);
	sMiscRenderer.Cone(mComponents.transforms.GetComponent(mHeadlights)->World(), mView, mProj, graphics);
}
