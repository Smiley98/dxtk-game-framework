#pragma once
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;
constexpr float DEGREES = 180.0f / DirectX::XM_PI;
constexpr float RADIANS = DirectX::XM_PI / 180.0f;

class Transform3
{
public:

	Matrix LocalTransform()
	{
		return Matrix::CreateScale(mScale) *
			Matrix::CreateFromQuaternion(mOrientation) *
			Matrix::CreateTranslation(mTranslation);

		//using namespace DirectX;
		//return XMMatrixTransformation(
		//	g_XMZero, Quaternion::Identity, mScale,
		//	g_XMZero, mOrientation,
		//	mTranslation);
	}

	Vector3 LocalPosition()
	{
		return DirectX::XMVector4Transform(Vector4::UnitW, LocalTransform());
		// return { LocalTransform() * Vector4::UnitW) };
		// No overload for matrix * vector.
		// XMVector4Transform() transposes the matrix. Perhaps this is to deal with DX vs OGL coordinates...
		// (manual scale * rotation * translation might not work).
	}

	Vector3 Forward()
	{
		return Matrix::CreateFromQuaternion(mOrientation).Forward();
	}

	Vector3 Backward()
	{
		return Matrix::CreateFromQuaternion(mOrientation).Backward();
	}

	Vector3 Left()
	{
		return Matrix::CreateFromQuaternion(mOrientation).Left();
	}

	Vector3 Right()
	{
		return Matrix::CreateFromQuaternion(mOrientation).Right();
	}

	Vector3 Up()
	{
		return Matrix::CreateFromQuaternion(mOrientation).Up();
	}

	Vector3 Down()
	{
		return Matrix::CreateFromQuaternion(mOrientation).Down();
	}

	Vector3 Translation()
	{
		return mTranslation;
	}

	Vector3 Rotation()
	{
		return mRotation * DEGREES;
	}

	Quaternion Orientation()
	{
		return mOrientation;
	}

	Vector3 Scaling()
	{
		return mScale;
	}

	void Translate(const Vector3& translation)
	{
		mTranslation = translation;
	}

	void Translate(const Vector2& translation)
	{
		mTranslation.x = translation.x;
		mTranslation.y = translation.y;
	}

	void TranslateX(float x)
	{
		mTranslation.x = x;
	}

	void TranslateY(float y)
	{
		mTranslation.y = y;
	}

	void TranslateZ(float z)
	{
		mTranslation.z = z;
	}

	void DeltaTranslate(const Vector3& translation)
	{
		mTranslation += translation;
	}

	void DeltaTranslate(const Vector2& translation)
	{
		mTranslation.x += translation.x;
		mTranslation.y += translation.y;
	}

	void DeltaTranslateX(float x)
	{
		mTranslation.x += x;
	}

	void DeltaTranslateY(float y)
	{
		mTranslation.y += y;
	}

	void DeltaTranslateZ(float z)
	{
		mTranslation.z += z;
	}

	void Rotate(const Vector3& degrees)
	{
		InternalDeltaRotate(degrees * RADIANS - mRotation);
	}

	void RotateX(float degrees)
	{
		float x = degrees * RADIANS;
		float dx = x - mRotation.x;
		InternalDeltaRotateX(dx);
	}

	void RotateY(float degrees)
	{
		float y = degrees * RADIANS;
		float dy = y - mRotation.y;
		InternalDeltaRotateY(dy);
	}

	void RotateZ(float degrees)
	{
		float z = degrees * RADIANS;
		float dz = z - mRotation.z;
		InternalDeltaRotateZ(dz);
	}

	void DeltaRotate(const Vector3& degrees)
	{
		InternalDeltaRotate(degrees * RADIANS);
	}

	void DeltaRotateX(float degrees)
	{
		float x = degrees * RADIANS;
		InternalDeltaRotateX(x);
	}

	void DeltaRotateY(float degrees)
	{
		float y = degrees * RADIANS;
		InternalDeltaRotateY(y);
	}

	void DeltaRotateZ(float degrees)
	{
		float z = degrees * RADIANS;
		InternalDeltaRotateZ(z);
	}

	void Scale(const Vector3& scale)
	{
		mScale = scale;
	}

private:
	Vector3 mTranslation = Vector3::Zero;
	Vector3 mRotation = Vector3::Zero;
	Vector3 mScale = Vector3::One;

	// Rotation of q1 followed by q2 = q2 * q1
	Quaternion mOrientation = Quaternion::Identity;

	void InternalDeltaRotate(const Vector3& radians)
	{
		Quaternion q2 = Quaternion::CreateFromYawPitchRoll(radians);
		Quaternion q1 = mOrientation;
		Quaternion::Concatenate(q1, q2, mOrientation);
		mRotation += radians;
	}

	void InternalDeltaRotateX(float radians)
	{
		Quaternion q2 = Quaternion::CreateFromYawPitchRoll(0.0f, radians, 0.0f);
		Quaternion q1 = mOrientation;
		Quaternion::Concatenate(q1, q2, mOrientation);
		mRotation.x += radians;
	}

	void InternalDeltaRotateY(float radians)
	{
		Quaternion q2 = Quaternion::CreateFromYawPitchRoll(radians, 0.0f, 0.0f);
		Quaternion q1 = mOrientation;
		Quaternion::Concatenate(q1, q2, mOrientation);
		mRotation.y += radians;
	}

	void InternalDeltaRotateZ(float radians)
	{
		Quaternion q2 = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, radians);
		Quaternion q1 = mOrientation;
		Quaternion::Concatenate(q1, q2, mOrientation);
		mRotation.z += radians;
	}
};