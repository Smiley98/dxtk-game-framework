#pragma once
#include "SimpleMath.h"
#include "MathConstants.h"

// Yes header namespace declarations are a "bad practice",
// but I'm on the verge of losing my mind every time I declare a DirectX::SimpleMath::Vector3...
using namespace DirectX::SimpleMath;

class TransformBase
{
	// Quaternion::LookRotation with Y-forward Z-Up
	inline Quaternion LookRotationInternal(const Vector3& forward, const Vector3& up);

public:
	// Set local forward (orientation)
	inline void SetForward(const Vector3& forward)
	{
		Vector3 euler = LookRotationInternal(forward, Vector3::UnitZ).ToEuler();
		mRotation.x = euler.x;
		mRotation.y = euler.z;
	}

	// Get local forward (orientation)
	inline Vector3 Forward() const
	{
		return Matrix::CreateFromYawPitchRoll({ mRotation.x, 0.0f, mRotation.y }).Up();
	}

	// Get local right (forward x up)
	inline Vector3 Right() const
	{
		return Forward().Cross(Vector3::UnitZ);
	}

	// Get local up (forward x right)
	inline Vector3 Up()	const
	{
		return Forward().Cross(Right());
	}

	// Get translation
	inline Vector3 Translation() const
	{
		return mTranslation;
	}

	// Get rotation (as euler angles in degrees)
	inline Vector2 Rotation() const
	{
		return mRotation * DirectX::XM_DEGREES;
	}

	inline float Pitch()
	{
		return mRotation.x;
	}

	inline float Yaw()
	{
		return mRotation.y;
	}

	// Set translation
	inline void Translate(const Vector3& translation)
	{
		mTranslation = translation;
	}

	// Append translation
	inline void DeltaTranslate(const Vector3& translation)
	{
		mTranslation += translation;
	}

	// Append 2d translation
	inline void DeltaTranslate(float x, float y)
	{
		mTranslation += { x, y, mTranslation.z };
	}

	// Set rotation as pitch and yaw
	inline void Rotate(const Vector2& degrees)
	{
		mRotation = degrees * DirectX::XM_RADIANS;
	}

	// Set pitch
	inline void SetPitch(float degrees)
	{
		mRotation.x = degrees * DirectX::XM_RADIANS;
	}

	// Set yaw
	inline void SetYaw(float degrees)
	{
		mRotation.y = degrees * DirectX::XM_RADIANS;
	}

	// Append pitch & yaw
	inline void DeltaRotate(const Vector2& degrees)
	{
		mRotation += degrees * DirectX::XM_RADIANS;
	}

	// Append pitch
	inline void DeltaPitch(float degrees)
	{
		mRotation.x += degrees * DirectX::XM_RADIANS;
	}

	// Append yaw
	inline void DeltaYaw(float degrees)
	{
		mRotation.y += degrees * DirectX::XM_RADIANS;
	}

protected:
	Vector3 mTranslation;
	Vector2 mRotation;	// x = pitch, y = yaw
};

// That awkward moment when a math library allows you to specify an up vector only to disregard it internally...
Quaternion TransformBase::LookRotationInternal(const Vector3& forward, const Vector3& up)
{
	Quaternion result;
	{
		using namespace DirectX;
		Quaternion q1;

		//Quaternion::FromToRotation(Vector3::Forward, forward, q1);
		Quaternion::FromToRotation(Vector3::UnitY, forward, q1);

		const XMVECTOR C = XMVector3Cross(forward, up);
		if (XMVector3NearEqual(XMVector3LengthSq(C), g_XMZero, g_XMEpsilon))
		{
			// forward and up are co-linear
			return q1;
		}

		//const XMVECTOR U = XMQuaternionMultiply(q1, Vector3::Up);
		const XMVECTOR U = XMQuaternionMultiply(q1, Vector3::UnitZ);

		Quaternion q2;
		Quaternion::FromToRotation(U, up, q2);

		XMStoreFloat4(&result, XMQuaternionMultiply(q2, q1));
	}
	return result;
}