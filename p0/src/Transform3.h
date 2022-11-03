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
			Matrix::CreateFromQuaternion(mOrientation) *
			Matrix::CreateTranslation(mTranslation);
	}

	Vector3 Forward()
	{
		return Vector3::Transform(Vector3::UnitZ, mOrientation);
	}

	Vector3 Right()
	{
		return Vector3::Transform(Vector3::UnitX, mOrientation);
	}

	Vector3 Up()
	{
		return Vector3::Transform(Vector3::UnitY, mOrientation);
	}

	Vector3 Translation()
	{
		return mTranslation;
	}

	Vector3 Rotation()
	{
		return mRotation * DirectX::XM_DEGREES;
	}

	Quaternion Orientation()
	{
		return mOrientation;
	}

	Vector3 Scaling()
	{
		return mScale;
	}

	void Orientate(const Quaternion& orientation)
	{
		mOrientation = orientation;
		mRotation = orientation.ToEuler();
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

	void Rotate(const Vector3& degrees)
	{
		InternalDeltaRotate(degrees * DirectX::XM_RADIANS - mRotation);
	}

	void Rotate(float degreesX, float degreesY, float degreesZ)
	{
		InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } *DirectX::XM_RADIANS - mRotation);
	}

	void RotateX(float degrees)
	{
		float x = degrees * DirectX::XM_RADIANS;
		float dx = x - mRotation.x;
		InternalDeltaRotateX(dx);
	}

	void RotateY(float degrees)
	{
		float y = degrees * DirectX::XM_RADIANS;
		float dy = y - mRotation.y;
		InternalDeltaRotateY(dy);
	}

	void RotateZ(float degrees)
	{
		float z = degrees * DirectX::XM_RADIANS;
		float dz = z - mRotation.z;
		InternalDeltaRotateZ(dz);
	}

	void DeltaRotate(const Vector3& degrees)
	{
		InternalDeltaRotate(degrees * DirectX::XM_RADIANS);
	}

	void DeltaRotate(float degreesX, float degreesY, float degreesZ)
	{
		InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } *DirectX::XM_RADIANS);
	}

	void DeltaRotateX(float degrees)
	{
		float x = degrees * DirectX::XM_RADIANS;
		InternalDeltaRotateX(x);
	}

	void DeltaRotateY(float degrees)
	{
		float y = degrees * DirectX::XM_RADIANS;
		InternalDeltaRotateY(y);
	}

	void DeltaRotateZ(float degrees)
	{
		float z = degrees * DirectX::XM_RADIANS;
		InternalDeltaRotateZ(z);
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
	// TODO -- since we have some room left, we should implement parenting here since we only need parent* and no children[].
	Vector3 mTranslation = Vector3::Zero;
	Vector3 mRotation = Vector3::Zero;
	Vector3 mScale = Vector3::One;

	Quaternion mOrientation = Quaternion::Identity;

	void InternalDeltaRotate(const Vector3& radians)
	{
		mOrientation *= Quaternion::CreateFromYawPitchRoll(radians);
		mRotation += radians;
		mRotation.x = fmodf(mRotation.x, DirectX::XM_2PI);
		mRotation.y = fmodf(mRotation.y, DirectX::XM_2PI);
		mRotation.z = fmodf(mRotation.z, DirectX::XM_2PI);
	}

	void InternalDeltaRotateX(float radians)
	{
		mOrientation *= Quaternion::CreateFromYawPitchRoll(0.0f, radians, 0.0f);
		mRotation.x += radians;
		mRotation.x = fmodf(mRotation.x, DirectX::XM_2PI);
	}

	void InternalDeltaRotateY(float radians)
	{
		mOrientation *= Quaternion::CreateFromYawPitchRoll(radians, 0.0f, 0.0f);
		mRotation.y += radians;
		mRotation.y = fmodf(mRotation.y, DirectX::XM_2PI);
	}

	void InternalDeltaRotateZ(float radians)
	{
		mOrientation *= Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, radians);
		mRotation.z += radians;
		mRotation.z = fmodf(mRotation.z, DirectX::XM_2PI);
	}
};

// Architecture:
// Optimizing for speed by storing translation, rotation and scale matrices + dirty flags doesn't make sense because
// it is not worth the 3x memory increase, adds complexity, and isn't as effective as you'd hope since data is being
// transported between float and vector registers.

// Math:
// Rotation of q1 followed by q2 = q2 * q1
// Concatenate(q1, q2) is equivalent to q2 * q1

// I should test in Unity to ensure quaternions actually behave the way I expect there!
/*
public Vector3 forward
{
	get
	{
		return rotation * Vector3.forward;
	}
	set
	{
		rotation = Quaternion.LookRotation(value);
	}
}
*/
