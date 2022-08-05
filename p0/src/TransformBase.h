#pragma once
#include "SimpleMath.h"
// Yes header namespace declarations are a "bad practice",
// but I'm on the verge of losing my mind every time I declare a DirectX::SimpleMath::Vector3...
using namespace DirectX::SimpleMath;

class TransformBase
{
public:

	// Local forward (orientation)
	inline Vector3 Front()	const
	{
		return Matrix::CreateFromYawPitchRoll(mRotation).Up();
	}

	// Local right (forward x up)
	inline Vector3 Adjacent() const
	{
		return Front().Cross(Vector3::UnitZ);
	}

	// Local up (forward x right)
	inline Vector3 Above()	const
	{
		return Front().Cross(Adjacent());
	}

	// Get translation
	inline Vector3 Translation() const
	{
		return mTranslation;
	}

	// Get rotation (as euler angles in degrees)
	inline Vector3 Rotation() const
	{
		constexpr float DEGREES = 180.0f / DirectX::XM_PI;
		return mRotation * DEGREES;
	}

	// Set translation
	inline void Translate(const Vector3& translation)
	{
		mTranslation = translation;
	}

	// Set rotation (as euler angles in degrees)
	inline void Rotate(const Vector3& degreesYXZ)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation = degreesYXZ * RADIANS;
	}

	// Append translation
	inline void DeltaTranslate(const Vector3& translation)
	{
		mTranslation += translation;
	}

	// Append rotation
	inline void DeltaRotate(const Vector3& degreesYXZ)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation += degreesYXZ * RADIANS;
	}

	// Append 2d translation
	inline void DeltaTranslate(float x, float y)
	{
		mTranslation += { x, y, mTranslation.z };
	}

	// Append 2d rotation
	inline void DeltaRotate(float degreesZ)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation.z += degreesZ * RADIANS;
	}

protected:
	Vector3 mTranslation;
	Vector3 mRotation;
};