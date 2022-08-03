#pragma once
#include "SimpleMath.h"

class RigidTransform;
class Transform
{
public:
	~Transform() {}
	Transform() :
		mTranslation(DirectX::SimpleMath::Vector3::Zero),
		mRotation(DirectX::SimpleMath::Vector3::Zero),
		mScaling(DirectX::SimpleMath::Vector3::One) {}

	inline operator RigidTransform() const;

	// World matrix
	inline DirectX::SimpleMath::Matrix World() const
	{
		using namespace DirectX;
		using namespace DirectX::SimpleMath;
		return XMMatrixTransformation(
			g_XMZero, Quaternion::Identity, mScaling,
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

	// Scale xyz
	inline DirectX::SimpleMath::Vector3 Scaling() const
	{
		return mScaling;
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

	inline void Scale(const DirectX::SimpleMath::Vector3& scaling)
	{
		mScaling = scaling;
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

	inline void DeltaScale(const DirectX::SimpleMath::Vector3& scaling)
	{
		mScaling += scaling;
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
	//inline void Scale(float scaling)
	//{
	//	mScaling = { scaling, scaling, scaling };
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
	//
	//inline void DeltaScale(float scaling)
	//{
	//	mScaling += { scaling, scaling, scaling };
	//}

private:
	DirectX::SimpleMath::Vector3 mTranslation;
	DirectX::SimpleMath::Vector3 mRotation;
	DirectX::SimpleMath::Vector3 mScaling;
};

#include "RigidTransform.h"

inline Transform::operator RigidTransform() const
{
	RigidTransform rigidTransform;
	rigidTransform.Rotate(Rotation());
	rigidTransform.Translate(Translation());
	return rigidTransform;
}

inline RigidTransform::operator Transform() const
{
	Transform transform;
	transform.Scale(DirectX::SimpleMath::Vector3::One);
	transform.Rotate(Rotation());
	transform.Translate(Translation());
	return transform;
}
