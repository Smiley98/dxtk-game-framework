#pragma once
#include "RigidTransform.h"

class Transform :
	public TransformBase
{
public:

	// World matrix
	inline Matrix World() const
	{
		using namespace DirectX;
		return XMMatrixTransformation(
			g_XMZero, Quaternion::Identity, mScaling,
			g_XMZero, Quaternion::CreateFromYawPitchRoll(mRotation),
			mTranslation);
	}

	// Position in world-space
	inline Vector4 Position() const {
		return DirectX::XMVector4Transform(Vector4::UnitW, World());
	}

	// Get scale
	inline Vector3 Scaling() const
	{
		return mScaling;
	}

	// Set scale
	inline void Scale(const Vector3& scaling)
	{
		mScaling = scaling;
	}

	// Append scale
	inline void DeltaScale(const Vector3& scaling)
	{
		mScaling += scaling;
	}
	
	// Append uniform scale
	inline void DeltaScale(float scaling)
	{
		mScaling += { scaling, scaling, scaling };
	}

	inline operator RigidTransform() const;

protected:
	Vector3 mScaling = Vector3::One;
};

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
	transform.Rotate(Rotation());
	transform.Translate(Translation());
	return transform;
}
