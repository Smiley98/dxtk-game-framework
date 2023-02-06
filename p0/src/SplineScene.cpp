#include "pch.h"
#include "SplineScene.h"
#include "EntityFunctions.h"
#include "PlayerSystem.h"
#include "DynamicsSystem.h"
#include "CollisionSystem.h"
#include "Steering.h"
#include "Dynamics.h"
#include "Utility.h"

using namespace DirectX;

namespace
{
	float r = 50.0f;

	// Player capsule collider dimensions
	constexpr float racerR = 33.0f;
	constexpr float racerHH = 43.0f;
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

	Vector3 startPosition = Vector3::Lerp(mSpline.points[0], mSpline.points[1], 0.5f);
	mRacer = CreateEntity(sComponents, startPosition + Vector3{ 0.0f, racerR / 3.0f, 0.0f });
	sComponents.rigidbodies.Add(mRacer).velocity = RandomCirclePoint(1.0f) * 100.0f;
	AddCapsule(mRacer, racerR, racerHH, sComponents);

	for (size_t i = 0; i < mSpline.points.size(); i++)
	{
		Vector3 A = mSpline.points[i];
		Vector3 B = mSpline.points[(i + 1) % mSpline.points.size()];
		mLines[i] = { A, B };
		mCheckpoints[i] = CreateEntity(sComponents, A);
		AddCapsule(mCheckpoints[i], 10.0f, 200.0f, sComponents);
		static float angle = 45.0f;
		sComponents.GetTransform(mCheckpoints[i]).RotateZ(angle);
		angle += 90.0f;
		mIntervals[i] = i;
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

	//for (size_t i = 0; i < 4; i++)
	//{
	//	if (Collision::IsColliding(mRacer, mCheckpoints[i], sComponents))
	//		mIntervals[0] = (i + 1) % 4;
	//}

	//EntityTransform& transform = sComponents.GetTransform(sPlayer);
	//Rigidbody& rb = sComponents.GetRigidbody(sPlayer);
	//mNearest = NearestProjection(transform.WorldPosition(), mSpline.points);
	//mFutureNearest = NearestProjection(
	//	transform.WorldPosition() + Dynamics::Integrate(rb.velocity, rb.acceleration, 0.5f), mSpline.points);
	FollowTrack(mRacer);
	Players::Update(sComponents, dt);
	Dynamics::Update(sComponents, dt);
}

void SplineScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
{
	for (size_t i = 0; i < 4; i++)
	{
		EntityTransform& transform = sComponents.GetTransform(mCheckpoints[i]);
		Collider& collider = sComponents.GetCollider(mCheckpoints[i]);
		//Debug::DrawCapsule(transform.WorldPosition(), transform.WorldForward(), collider.r, collider.hh, Colors::White, true);
		Debug::DrawLine(mLines[i].start, mLines[i].end);
	}

	sPlayerRenderer.Render(sComponents.GetTransform(sPlayer).World(), mView, mProj, graphics);
	sPlayerRenderer.Render(sComponents.GetTransform(mRacer).World(), mView, mProj, graphics);
}

void SplineScene::FollowTrack(Entity& entity)
{
	size_t index = 0;
	EntityTransform& transform = sComponents.GetTransform(entity);
	Vector3 position = transform.WorldPosition();
	Vector3 nearest = NearestProjection(position, mSpline.points, index);
	index = SphereSphere(nearest, mLines[index].end, 10.0f, 10.0f) ? (index + 1) % mLines.size() : index;
	Vector3 toEnd = mLines[index].end - nearest;
	toEnd.Normalize();

	static const float predictionTime = 0.5f;
	Rigidbody& rb = sComponents.GetRigidbody(entity);
	Vector3 prediction = Project(mLines[index], position + Dynamics::Integrate(rb.velocity.Length() * toEnd, rb.acceleration, predictionTime));
	rb.acceleration = Steering::Seek(prediction, position, rb.velocity, 500.0f);
}

/*
//std::array<Entity, 4> mBounds;
// No need for bounds because racers seek the track regardless of how off-course they are.
// Would also need to create bounds along the edges of the track instead of encompassing the track
// 
//Vector3 centre = Vector3::Lerp(A, B, 0.5f);
//Vector3 direction = B - A;
//direction.Normalize();
//Entity bounds = CreateEntity(sComponents, centre);
//AddCapsule(bounds, r * 2.0f, (centre - A).Length(), sComponents);
//sComponents.GetTransform(bounds).Orientate(direction);
//mBounds[i] = bounds;
// 
//EntityTransform& transform = sComponents.GetTransform(mBounds[i]);
//Collider& collider = sComponents.GetCollider(mBounds[i]);
//Debug::DrawCapsule(transform.WorldPosition(), transform.WorldForward(), collider.r, collider.hh, Colors::White, true);
*/
