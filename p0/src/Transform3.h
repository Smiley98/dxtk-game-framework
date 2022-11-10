#pragma once
#include "SimpleMath.h"
#include "MathConstants.h"
#include "Utility.h"

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

	void Rotate(float degreesX, float degreesY, float degreesZ)
	{
		Rotate({ degreesX, degreesY, degreesZ });
	}

	void Rotate(const Vector3& degrees)
	{
		Vector3 delta = degrees * DirectX::XM_RADIANS - mEuler;
		mRotation *= Quaternion::CreateFromYawPitchRoll(delta);
		mEuler += delta;
	}

	void RotateX(float degreesX)
	{
		float deltaX = degreesX * DirectX::XM_RADIANS - mEuler.x;
		mRotation *= Quaternion::CreateFromYawPitchRoll(0.0f, deltaX, 0.0f);
		mEuler.x += deltaX;
	}

	void RotateY(float degreesY)
	{
		float deltaY = degreesY * DirectX::XM_RADIANS - mEuler.y;
		mRotation *= Quaternion::CreateFromYawPitchRoll(deltaY, 0.0f, 0.0f);
		mEuler.y += deltaY;
	}

	void RotateZ(float degreesZ)
	{
		float deltaZ = degreesZ * DirectX::XM_RADIANS - mEuler.z;
		mRotation *= Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, deltaZ);
		mEuler.z += deltaZ;
	}

	void DeltaRotate(float degreesX, float degreesY, float degreesZ)
	{
		DeltaRotate({ degreesX, degreesY, degreesZ });
	}

	void DeltaRotate(const Vector3& degrees)
	{
		mRotation *= Quaternion::CreateFromYawPitchRoll(degrees * DirectX::XM_RADIANS);
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

	static Quaternion Delta(Quaternion from, const Quaternion& to)
	{
		from.Conjugate();
		return Quaternion::Concatenate(from, to);
	}

private:
	Transform* mParent = nullptr;
	Quaternion mRotation = Quaternion::Identity;
	Vector3 mTranslation = Vector3::Zero;
	Vector3 mEuler = Vector3::Zero;
	Vector3 mScale = Vector3::One;
};
