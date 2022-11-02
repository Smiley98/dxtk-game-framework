#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

// These classes seem too small... Might as well just use SM and XM directly at this point!
/*struct Transform2
{
	virtual Matrix Transformation() const = 0;

	Vector3 Forward() const
	{
		return Vector3::Transform(Vector3::UnitZ, rotation);
	}

	Vector3 Right() const
	{
		return Vector3::Transform(Vector3::UnitX, rotation);
	}

	Vector3 Up() const
	{
		return Vector3::Transform(Vector3::UnitY, rotation);
	}

	Quaternion rotation = Quaternion::Identity;
	Vector3 translation = Vector3::Zero;
};

struct RigidTransform2 : Transform2
{
	Matrix Transformation() const final
	{
		return Matrix::CreateFromQuaternion(rotation) *
			Matrix::CreateTranslation(translation);
	}
};

struct AffineTransform2 : Transform2
{
	Matrix Transformation() const final
	{
		return Matrix::CreateScale(scale) *
			Matrix::CreateFromQuaternion(rotation) *
			Matrix::CreateTranslation(translation);
	}

	Vector3 scale = Vector3::One;
};*/
