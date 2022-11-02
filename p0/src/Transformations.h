#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

#define Y_FORWARD false
#define Z_FORWARD true

struct RigidTransform2
{
	//DirectX::SimpleMath::Vector3 translation = DirectX::SimpleMath::Vector3::Zero;
	//DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::Identity;
	Vector3 translation = Vector3::Zero;
	Quaternion rotation = Quaternion::Identity;
};

Matrix RigidTransformation(const RigidTransform2& rigidTransform)
{
	return Matrix::CreateFromQuaternion(rigidTransform.rotation) *
		Matrix::CreateTranslation(rigidTransform.translation);
}

struct AffineTransform2
{
	//DirectX::SimpleMath::Vector3 translation = DirectX::SimpleMath::Vector3::Zero;
	//DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::Identity;
	//DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3::One;

	Vector3 translation = Vector3::Zero;
	Quaternion rotation = Quaternion::Identity;
	Vector3 scale = Vector3::One;
};

Matrix AffineTransformation(const AffineTransform2& affineTransformation)
{
	return Matrix::CreateScale(affineTransformation.scale) *
		Matrix::CreateFromQuaternion(affineTransformation.rotation) *
		Matrix::CreateTranslation(affineTransformation.translation);
}

Vector3 Forward(const Quaternion& rotation)
{
#if Y_FORWARD
	return Vector3::Transform(Vector3::Up, rotation);
#endif

#if Z_FORWARD
	return Vector3::Transform(Vector3::Forward, rotation);
#endif
}

Vector3 Right(const Quaternion& rotation)
{
	// X is right regardless so only Y and Z need to be swapped between forward and up.
	// Not supporting other permutations such as Z-forward as seen in LHS (using RHS).
	return Vector3::Transform(Vector3::Right, rotation);
}

Vector3 Up(const Quaternion& rotation)
{
#if Y_FORWARD
	return Vector3::Transform(Vector3::Forward, rotation);
#endif

#if Z_FORWARD
	return Vector3::Transform(Vector3::Up, rotation);
#endif
}
