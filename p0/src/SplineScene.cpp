#include "pch.h"
#include "SplineScene.h"
#include "Utility.h"

using namespace DirectX;

SplineScene::SplineScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components)
	: Scene(graphics, audio, components)
{
	mHeadlights = CreateEntity(mComponents);
	mComponents.transforms.GetComponent(mHeadlights)->TranslateY(80.0f);
	mComponents.transforms.GetComponent(mHeadlights)->Scale(100.0f);
	AddChild(sPlayer, mHeadlights, mComponents);

	mSpeedTable = CreateSpeedTable(mSpline, 16);
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
	static size_t interval = 0;
	static size_t sample = 0;
	static float distance = 0.0f;
	Vector3 a = Catmull(DistanceToInterpolation(distance, mSpeedTable, interval, sample), interval, mSpline);
	distance += 250.0f * dt;
	UpdateCatmull(distance, interval, sample, mSpline, mSpeedTable);
	Vector3 b = Catmull(DistanceToInterpolation(distance, mSpeedTable, interval, sample), interval, mSpline);
	Vector3 forward = b - a;
	forward.Normalize();
	EntityTransform& transform = *mComponents.transforms.GetComponent(sPlayer);
	transform.Translate(a);
	transform.Orientate(forward);

	Vector3 position = transform.WorldPosition();
	float shortestDistance = std::numeric_limits<float>::max();
	for (size_t i = 1; i <= mSpline.size(); i++)
	{
		Vector3 projection = Project(mSpline[i - 1], mSpline[i % mSpline.size()], position);
		float distance = (position - projection).LengthSquared();
		if (distance < shortestDistance)
		{
			shortestDistance = distance;
			mNearest = projection;
		}
	}
}

void SplineScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	for (const Vector3& position : mSpline)
		Debug::Sphere(position, 50.0f, mView, mProj, graphics);

	for (size_t i = 1; i <= mSpline.size(); i++)
		Debug::Line(mSpline[i - 1], mSpline[i % mSpline.size()], 10.0f, mView, mProj, graphics);

	Debug::Sphere(mNearest, 50.0f, mView, mProj, graphics);
	sPlayerRenderer.Render(mComponents.transforms.GetComponent(sPlayer)->World(), mView, mProj, graphics);
	sMiscRenderer.Cone(mComponents.transforms.GetComponent(mHeadlights)->World(), mView, mProj, graphics);
}
