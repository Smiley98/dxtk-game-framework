#pragma once
#include "SimpleMath.h"
// Yes header namespace declarations are a "bad practice",
// but I'm on the verge of losing my mind every time I declare a DirectX::SimpleMath::Vector3...
using namespace DirectX::SimpleMath;

class TransformBase
{
public:

	// WIP
	//inline void Orientate(const Vector3& forward)
	//{
	//	// This function "works" if only y is used. I'm at a loss for decomposing a direction to pitch and yaw...
	//	Vector3 euler = Matrix::CreateWorld(mTranslation, forward, Vector3::UnitZ).ToEuler();
	//	//mRotation.x = euler.y;
	//	mRotation.y = euler.x;
	//}

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
		constexpr float DEGREES = 180.0f / DirectX::XM_PI;
		return mRotation * DEGREES;
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
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation = degrees * RADIANS;
	}

	// Set pitch
	inline void SetPitch(float degrees)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation.x = degrees * RADIANS;
	}

	// Set yaw
	inline void SetYaw(float degrees)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation.y = degrees * RADIANS;
	}

	// Append pitch & yaw
	inline void DeltaRotate(const Vector2& degrees)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation += degrees * RADIANS;
	}

	// Append pitch
	inline void DeltaPitch(float degrees)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation.x += degrees * RADIANS;
	}

	// Append yaw
	inline void DeltaYaw(float degrees)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation.y += degrees * RADIANS;
	}

protected:
	Vector3 mTranslation;
	Vector2 mRotation;	// x = pitch, y = yaw
};