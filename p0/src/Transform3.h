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

	//Vector3 Rotation()
	//{
	//	return mRotation * DirectX::XM_DEGREES;
	//}

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
		//mRotation = orientation.ToEuler();
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
		Quaternion to = Quaternion::CreateFromYawPitchRoll(degrees * DirectX::XM_RADIANS);
		Quaternion inverseFrom = mOrientation;
		inverseFrom.Conjugate();
		Quaternion delta = Quaternion::Concatenate(inverseFrom, to);
		mOrientation *= delta;

		//mOrientation = Quaternion::CreateFromYawPitchRoll(degrees * DirectX::XM_RADIANS);
		//InternalDeltaRotate(degrees * DirectX::XM_RADIANS - mRotation);
	}

	void Rotate(float degreesX, float degreesY, float degreesZ)
	{
		Rotate({ degreesX, degreesY, degreesZ });
		//InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } * DirectX::XM_RADIANS - mRotation);
	}

	void RotateX(float degrees)
	{
		mOrientation = Quaternion::CreateFromYawPitchRoll(0.0f, degrees * DirectX::XM_RADIANS, 0.0f);
		//float x = degrees * DirectX::XM_RADIANS;
		//float dx = x - mRotation.x;
		//InternalDeltaRotateX(dx);
	}

	void RotateY(float degrees)
	{
		mOrientation = Quaternion::CreateFromYawPitchRoll(degrees * DirectX::XM_RADIANS, 0.0f, 0.0f);
		//float y = degrees * DirectX::XM_RADIANS;
		//float dy = y - mRotation.y;
		//InternalDeltaRotateY(dy);
	}

	void RotateZ(float degrees)
	{
		mOrientation = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, degrees * DirectX::XM_RADIANS);
		//float z = degrees * DirectX::XM_RADIANS;
		//float dz = z - mRotation.z;
		//InternalDeltaRotateZ(dz);
	}

	void DeltaRotate(const Vector3& degrees)
	{
		mOrientation *= Quaternion::CreateFromYawPitchRoll(degrees * DirectX::XM_RADIANS);
		//InternalDeltaRotate(degrees * DirectX::XM_RADIANS);
	}

	void DeltaRotate(float degreesX, float degreesY, float degreesZ)
	{
		DeltaRotate({ degreesX, degreesY, degreesZ });
		//InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } *DirectX::XM_RADIANS);
	}

	void DeltaRotateX(float degrees)
	{
		mOrientation *= Quaternion::CreateFromYawPitchRoll(0.0f, degrees * DirectX::XM_RADIANS, 0.0f);
		//float x = degrees * DirectX::XM_RADIANS;
		//InternalDeltaRotateX(x);
	}

	void DeltaRotateY(float degrees)
	{
		mOrientation *= Quaternion::CreateFromYawPitchRoll(degrees * DirectX::XM_RADIANS, 0.0f, 0.0f);
		//float y = degrees * DirectX::XM_RADIANS;
		//InternalDeltaRotateY(y);
	}

	void DeltaRotateZ(float degrees)
	{
		mOrientation *= Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, degrees * DirectX::XM_RADIANS);
		//float z = degrees * DirectX::XM_RADIANS;
		//InternalDeltaRotateZ(z);
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
	Quaternion mOrientation = Quaternion::Identity;
	Vector3 mTranslation = Vector3::Zero;
	Vector3 mScale = Vector3::One;
	Transform* mParent = nullptr;
};

// Architecture:
// Optimizing for speed by storing translation, rotation and scale matrices + dirty flags doesn't make sense because
// it is not worth the 3x memory increase, adds complexity, and isn't as effective as you'd hope since data is being
// transported between float and vector registers.

// Math:
// Rotation of q1 followed by q2 = q2 * q1
// Concatenate(q1, q2) is equivalent to q2 * q1

// Removed euler capabilities because you can't add/subtract eulers and overall they're not useful.
/*
Vector3 mRotation = Vector3::Zero;
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
*/
