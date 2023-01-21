#pragma once
#include "MathUtilities.h"
#define Y_FORWARD true

class Transform
{
public:
//***************
// Local getters
//***************
	Matrix Local() const
	{
		return Matrix::CreateScale(mScale) *
			Matrix::CreateFromQuaternion(mRotation) *
			Matrix::CreateTranslation(mTranslation);
	}

	Vector3 Forward() const
	{
#if Y_FORWARD
		return Vector3::Transform(Vector3::UnitY, mRotation);
#else
		return Vector3::Transform(Vector3::UnitZ, mRotation);
#endif
	}

	Vector3 Right() const
	{
		return Vector3::Transform(Vector3::UnitX, mRotation);
	}

	Vector3 Up() const
	{
#if Y_FORWARD
		return Vector3::Transform(Vector3::UnitZ, mRotation);
#else
		return Vector3::Transform(Vector3::UnitY, mRotation);
#endif
	}

	Vector3 Translation() const
	{
		return mTranslation;
	}

	Quaternion Rotation() const
	{
		return mRotation;
	}

	Vector3 Euler() const
	{
		return mEuler * DirectX::XM_DEGREES;
	}

	Vector3 Scaling() const
	{
		return mScale;
	}

//***************
// Local setters
//***************
	void Translate(const Vector3& translation)
	{
		mTranslation = translation;
	}

	void Translate(float x, float y, float z)
	{
		mTranslation = { x, y, z };
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

	void DeltaTranslate(float x, float y, float z)
	{
		mTranslation += { x, y, z };
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

	void Orientate(const Vector3& forward)
	{
#if Y_FORWARD
		Vector3 right = forward.Cross(Vector3::UnitZ);
		Vector3 up = forward.Cross(right);
		Matrix orientation(right, forward, -up);
#else
		Vector3 right = forward.Cross(Vector3::UnitY);
		Vector3 up = forward.Cross(right);
		Matrix orientation(right, up, forward);
#endif
		mRotation = Quaternion::CreateFromRotationMatrix(orientation);
		mEuler = mRotation.ToEuler();
	}

	void Rotate(const Quaternion& rotation)
	{
		mRotation = rotation;
		mEuler = rotation.ToEuler();
	}

	void Rotate(const Vector3& degrees)
	{
		InternalDeltaRotate(degrees * DirectX::XM_RADIANS - mEuler);
	}

	void Rotate(float degreesX, float degreesY, float degreesZ)
	{
		InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } * DirectX::XM_RADIANS - mEuler);
	}

	void RotateX(float degreesX)
	{
		InternalDeltaRotate({ degreesX * DirectX::XM_RADIANS - mEuler.x, 0.0f, 0.0f });
	}

	void RotateY(float degreesY)
	{
		InternalDeltaRotate({ 0.0f, degreesY * DirectX::XM_RADIANS - mEuler.y, 0.0f });
	}

	void RotateZ(float degreesZ)
	{
		InternalDeltaRotate({ 0.0f, 0.0f, degreesZ * DirectX::XM_RADIANS - mEuler.z });
	}

	void DeltaRotate(const Quaternion& rotation)
	{
		InternalDeltaRotate(rotation.ToEuler());
	}

	void DeltaRotate(const Vector3& degrees)
	{
		InternalDeltaRotate(degrees * DirectX::XM_RADIANS);
	}

	void DeltaRotate(float degreesX, float degreesY, float degreesZ)
	{
		InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } * DirectX::XM_RADIANS);
	}

	void DeltaRotateX(float degreesX)
	{
		InternalDeltaRotate({ degreesX * DirectX::XM_RADIANS, 0.0f, 0.0f });
	}

	void DeltaRotateY(float degreesY)
	{
		InternalDeltaRotate({ 0.0f, degreesY * DirectX::XM_RADIANS, 0.0f });
	}

	void DeltaRotateZ(float degreesZ)
	{
		InternalDeltaRotate({ 0.0f, 0.0f, degreesZ * DirectX::XM_RADIANS });
	}

	void Scale(const Vector3& scale)
	{
		mScale = scale;
	}

	void Scale(float scale)
	{
		mScale = { scale };
	}

	void ScaleX(float x)
	{
		mScale.x = x;
	}

	void ScaleY(float y)
	{
		mScale.y = y;
	}

	void ScaleZ(float z)
	{
		mScale.z = z;
	}

protected:
	Quaternion mRotation = Quaternion::Identity;
	Vector3 mTranslation = Vector3::Zero;
	Vector3 mEuler = Vector3::Zero;
	Vector3 mScale = Vector3::One;

	void InternalDeltaRotate(const Vector3& radians)
	{
		mRotation *= Quaternion::CreateFromYawPitchRoll(radians);
		mEuler += radians;
	}
};
