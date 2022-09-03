#pragma once
#include "TransformBase.h"

class Transform;
class RigidTransform :
	public TransformBase
{
public:

	// World matrix
	inline DirectX::SimpleMath::Matrix World() const
	{
		using namespace DirectX;
		return XMMatrixTransformation(
			g_XMZero, Quaternion::Identity, Vector3::One,
			g_XMZero, Quaternion::CreateFromYawPitchRoll({ mRotation.x, 0.0f, mRotation.y }),
			mTranslation);
	}

	// Position in world-space
	inline Vector4 Position() const {
		return DirectX::XMVector4Transform(Vector4::UnitW, World());
	}

	inline operator Transform() const;
};