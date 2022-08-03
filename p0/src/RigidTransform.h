#pragma once
#include "SimpleMath.h"

class Transform;
class RigidTransform
{
public:
	~RigidTransform() {}
	RigidTransform() :
		mTranslation(DirectX::SimpleMath::Vector3::Zero),
		mRotation(DirectX::SimpleMath::Vector3::Zero) {}

	inline operator Transform() const;

	// World matrix
	inline DirectX::SimpleMath::Matrix World() const
	{
		using namespace DirectX;
		using namespace DirectX::SimpleMath;
		return XMMatrixTransformation(
			g_XMZero, Quaternion::Identity, Vector3::One,
			g_XMZero, Quaternion::CreateFromYawPitchRoll(mRotation),
			mTranslation);
	}

	// Position in world-space
	inline DirectX::SimpleMath::Vector4 Position() const {
		using namespace DirectX;
		using namespace DirectX::SimpleMath;
		return XMVector4Transform(Vector4::UnitW, World());
	}

	// local forward (orientation)
	inline DirectX::SimpleMath::Vector3 Front()	const
	{
		using namespace DirectX::SimpleMath;
		return Matrix::CreateFromYawPitchRoll(mRotation).Up();
	}

	// local right (forward x up)
	inline DirectX::SimpleMath::Vector3 Adjacent() const
	{
		using namespace DirectX::SimpleMath;
		return Front().Cross(Vector3::UnitZ);
	}

	// local up (forward x right)
	inline DirectX::SimpleMath::Vector3 Above()	const
	{
		return Front().Cross(Adjacent());
	}

	// Translation
	inline DirectX::SimpleMath::Vector3 Translation() const
	{
		return mTranslation;
	}

	// Euler angles in degrees
	inline DirectX::SimpleMath::Vector3 Rotation() const
	{
		constexpr float DEGREES = 180.0f / DirectX::XM_PI;
		return mRotation * DEGREES;
	}

	inline void Translate(const DirectX::SimpleMath::Vector3& translation)
	{
		mTranslation = translation;
	}

	inline void Rotate(const DirectX::SimpleMath::Vector3& degreesYXZ)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation = degreesYXZ * RADIANS;
	}

	inline void DeltaTranslate(const DirectX::SimpleMath::Vector3& translation)
	{
		mTranslation += translation;
	}

	inline void DeltaRotate(const DirectX::SimpleMath::Vector3& degreesYXZ)
	{
		constexpr float RADIANS = DirectX::XM_PI / 180.0f;
		mRotation += degreesYXZ * RADIANS;
	}

	// These 2d convenience methods are ambiguous because its not clear what happens to z;
	// Does z get overwritten to 0 since x and y are overwritten, or does it persist?
	//inline void Translate(float x, float y)
	//{
	//	mTranslation = { x, y, mTranslation.z };
	//}
	//
	//inline void Rotate(float degreesZ)
	//{
	//	constexpr float RADIANS = DirectX::XM_PI / 180.0f;
	//	mRotation.z = degreesZ * RADIANS;
	//}
	//
	//inline void DeltaTranslate(float x, float y)
	//{
	//	mTranslation += { x, y, mTranslation.z };
	//}
	//
	//inline void DeltaRotate(float degreesZ)
	//{
	//	constexpr float RADIANS = DirectX::XM_PI / 180.0f;
	//	mRotation.z += degreesZ * RADIANS;
	//}

private:
	DirectX::SimpleMath::Vector3 mTranslation;
	DirectX::SimpleMath::Vector3 mRotation;
};