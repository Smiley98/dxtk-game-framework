#pragma once
#include "SimpleMath.h"
#include "MathConstants.h"

using namespace DirectX::SimpleMath;

class Transform3
{
public:

	Matrix LocalMatrix()
	{
		return Matrix::CreateScale(mScale) *
			Matrix::CreateFromQuaternion(mRotation) *
			Matrix::CreateTranslation(mTranslation);
	}

	Vector3 Forward()
	{
		return Vector3::Transform(Vector3::UnitZ, mRotation);
	}

	Vector3 Right()
	{
		return Vector3::Transform(Vector3::UnitX, mRotation);
	}

	Vector3 Up()
	{
		return Vector3::Transform(Vector3::UnitY, mRotation);
	}

	Vector3 Translation()
	{
		return mTranslation;
	}

	Quaternion Rotation()
	{
		return mRotation;
	}

	Vector3 Scaling()
	{
		return mScale;
	}

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

	void Rotate(const Quaternion& rotation)
	{
		mRotation = rotation;
	}

	void Rotate(const Vector3& degrees)
	{
		Quaternion to = Quaternion::CreateFromYawPitchRoll(degrees * DirectX::XM_RADIANS);
		Quaternion inverseFrom = mRotation;
		inverseFrom.Conjugate();
		Quaternion delta = Quaternion::Concatenate(inverseFrom, to);
		mRotation *= delta;
		//mRotation = Quaternion::CreateFromYawPitchRoll(degrees * DirectX::XM_RADIANS);
	}

	void Rotate(float degreesX, float degreesY, float degreesZ)
	{
		Rotate({ degreesX, degreesY, degreesZ });
	}

	void RotateX(float degreesX)
	{
		mRotation = Quaternion::CreateFromYawPitchRoll(0.0f, degreesX * DirectX::XM_RADIANS, 0.0f);
	}

	void RotateY(float degreesY)
	{
		mRotation = Quaternion::CreateFromYawPitchRoll(degreesY * DirectX::XM_RADIANS, 0.0f, 0.0f);
	}

	void RotateZ(float degreesZ)
	{
		mRotation = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, degreesZ * DirectX::XM_RADIANS);
	}

	void DeltaRotate(const Vector3& degrees)
	{
		mRotation *= Quaternion::CreateFromYawPitchRoll(degrees * DirectX::XM_RADIANS);
	}

	void DeltaRotate(float degreesX, float degreesY, float degreesZ)
	{
		DeltaRotate({ degreesX, degreesY, degreesZ });
	}

	void DeltaRotateX(float degreesX)
	{
		mRotation *= Quaternion::CreateFromYawPitchRoll(0.0f, degreesX * DirectX::XM_RADIANS, 0.0f);
	}

	void DeltaRotateY(float degreesY)
	{
		mRotation *= Quaternion::CreateFromYawPitchRoll(degreesY * DirectX::XM_RADIANS, 0.0f, 0.0f);
	}

	void DeltaRotateZ(float degreesZ)
	{
		mRotation *= Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, degreesZ * DirectX::XM_RADIANS);
	}

	void Scale(const Vector3& scale)
	{
		mScale = scale;
	}

	void Scale(float scale)
	{
		mScale = { scale };
	}

private:
	Quaternion mRotation = Quaternion::Identity;
	Vector3 mTranslation = Vector3::Zero;
	Vector3 mScale = Vector3::One;
	Transform* mParent = nullptr;
};
