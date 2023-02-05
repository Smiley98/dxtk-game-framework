#include "pch.h"
#include "SplineScene.h"
#include "EntityFunctions.h"
#include "PlayerSystem.h"
#include "DynamicsSystem.h"
#include "Dynamics.h"
#include "Utility.h"

using namespace DirectX;

namespace
{
	float r = 50.0f;
}

SplineScene::SplineScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	: Scene(graphics, audio)
{
	mSpline.points = {
		Vector3{ 500.0f, -300.0f, 0.0f },
		Vector3{ -500.0f, -300.0f, 0.0f },
		Vector3{ -500.0f, 300.0f, 0.0f },
		Vector3{ 500.0f, 300.0f, -0.0f }
	};
	mSpline.speedTable = CreateSpeedTable(mSpline.points, 16);

	for (size_t i = 0; i < mSpline.points.size(); i++)
	{
		Vector3 A = mSpline.points[i];
		Vector3 B = mSpline.points[(i + 1) % mSpline.points.size()];
		mTrack.lines[i] = { A, B };

		Vector3 centre = Vector3::Lerp(A, B, 0.5f);
		Vector3 direction = B - A;
		direction.Normalize();

		Entity bounds = CreateEntity(sComponents, centre);
		AddCapsule(bounds, r, (centre - A).Length(), sComponents);
		sComponents.GetTransform(bounds).Orientate(direction);
		mTrack.bounds[i] = bounds;
	}
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
	sComponents.GetTransform(sPlayer).Translate(mSpline.points[0]);
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

void SplineScene::OnUpdate(float dt, float tt)
{
	//static float lv = 250.0f;
	//FollowPath(dt, lv, mSpline, sPlayer, sComponents);

	EntityTransform& transform = sComponents.GetTransform(sPlayer);
	Rigidbody& rb = sComponents.GetRigidbody(sPlayer);
	mNearest = NearestProjection(transform.WorldPosition(), mSpline.points);
	mFutureNearest = NearestProjection(
		transform.WorldPosition() + Dynamics::Integrate(rb.velocity, rb.acceleration, 0.5f), mSpline.points);
	
	Players::Update(sComponents, dt);
	Dynamics::Update(sComponents, dt);
}

void SplineScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	//for (size_t i = 0; i < 4; i++)
	//{
	//	Vector3 current = mSpline.points[i];
	//	Vector3 next = mSpline.points[(i + 1) % mSpline.points.size()];
	//	Debug::DrawSphere(current, r);
	//	Debug::DrawLine(current, next, 10.0f);
	//	Debug::DrawSphere(Project(current, next, sComponents.GetTransform(sPlayer).WorldPosition()), r);
	//}

	for (size_t i = 0; i < 4; i++)
	{
		EntityTransform& transform = sComponents.GetTransform(mTrack.bounds[i]);
		Collider& collider = sComponents.GetCollider(mTrack.bounds[i]);
		Debug::DrawLine(mTrack.lines[i].start, mTrack.lines[i].end);
		Debug::DrawCapsule(transform.WorldPosition(), transform.WorldForward(), r, collider.hh, Colors::White, true);
	}

	Debug::DrawSphere(mNearest, r);
	Debug::DrawSphere(mFutureNearest, r);
	sPlayerRenderer.Render(sComponents.GetTransform(sPlayer).World(), mView, mProj, graphics);
}

//mHeadlights = CreateEntity(mComponents);
//mComponents.GetTransform(mHeadlights).TranslateY(80.0f);
//mComponents.GetTransform(mHeadlights).Scale(100.0f);
//sMiscRenderer.Cone(mComponents.GetTransform(mHeadlights)->World(), mView, mProj, graphics);
