#include "pch.h"
#include "Transform.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

constexpr float RADIANS = XM_PI / 180.0f;
constexpr float DEGREES = 180.0f / XM_PI;

Transform::Transform() :
	mTranslation(Vector3::Zero),
	mRotation(Vector3::Zero),
	mScaling(Vector3::One)
{
}

Transform::~Transform()
{
}

DirectX::SimpleMath::Matrix Transform::World()
{
	return XMMatrixTransformation(
		g_XMZero, Quaternion::Identity, mScaling,
		g_XMZero, Quaternion::CreateFromYawPitchRoll(mRotation),
		mTranslation);
}

DirectX::SimpleMath::Vector4 Transform::Position()
{
	return XMVector4Transform(Vector4::UnitW, World());
}

DirectX::SimpleMath::Vector3 Transform::Front()
{
	// front = rotation[1]
	return Matrix::CreateFromYawPitchRoll(mRotation).Up();
}

DirectX::SimpleMath::Vector3 Transform::Adjacent()
{
	// right = front x up
	return Front().Cross(Vector3::UnitZ);
}

DirectX::SimpleMath::Vector3 Transform::Above()
{
	// up = front x right
	return Front().Cross(Adjacent());
}

DirectX::SimpleMath::Vector3 Transform::Translation()
{
	return mTranslation;
}

DirectX::SimpleMath::Vector3 Transform::Rotation()
{
	return mRotation * DEGREES;
}

DirectX::SimpleMath::Vector3 Transform::Scaling()
{
	return mScaling;
}

void Transform::Translate(const DirectX::SimpleMath::Vector3& translation)
{
	mTranslation = translation;
}

void Transform::Translate(float x, float y)
{
	mTranslation = { x, y, mTranslation.z };
}

void Transform::Rotate(const DirectX::SimpleMath::Vector3& degreesYXZ)
{
	mRotation = degreesYXZ * RADIANS;
}

void Transform::Rotate(float degreesZ)
{
	mRotation.z = degreesZ * RADIANS;
}

void Transform::Scale(const DirectX::SimpleMath::Vector3& scaling)
{
	
	mScaling = scaling;
}

void Transform::Scale(float scaling)
{
	mScaling = { scaling, scaling, scaling };
}

void Transform::DeltaTranslate(const DirectX::SimpleMath::Vector3& translation)
{
	mTranslation += translation;
}

void Transform::DeltaTranslate(float x, float y)
{
	mTranslation += { x, y, mTranslation.z };
}

void Transform::DeltaRotate(const DirectX::SimpleMath::Vector3& degreesYXZ)
{
	mRotation += degreesYXZ * RADIANS;
}

void Transform::DeltaRotate(float degreesZ)
{
	mRotation.z += degreesZ * RADIANS;
}

void Transform::DeltaScale(const DirectX::SimpleMath::Vector3& scaling)
{
	mScaling += scaling;
}

void Transform::DeltaScale(float scaling)
{
	mScaling += { scaling, scaling, scaling };
}

