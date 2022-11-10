#pragma once
#include "SimpleMath.h"
#include "MathConstants.h"
#include "Utility.h"

using namespace DirectX::SimpleMath;

class Transform3
{
public:
	Transform3* const Parent() const
	{
		return mParent;
	}

	void SetParent(Transform3* parent)
	{
		mParent = parent;
	}

//***************
// Local methods
//***************
	Matrix Local() const
	{
		return Matrix::CreateScale(mScale) *
			Matrix::CreateFromQuaternion(mRotation) *
			Matrix::CreateTranslation(mTranslation);
	}

	Vector3 Forward() const
	{
		return Vector3::Transform(Vector3::UnitZ, mRotation);
	}

	Vector3 Right() const
	{
		return Vector3::Transform(Vector3::UnitX, mRotation);
	}

	Vector3 Up() const
	{
		return Vector3::Transform(Vector3::UnitY, mRotation);
	}

	Vector3 Translation() const
	{
		return mTranslation;
	}

	Quaternion Rotation() const
	{
		return mRotation;
	}

	Vector3 Euler() const
	{
		return mEuler * DirectX::XM_DEGREES;
	}

	Vector3 Scaling() const
	{
		return mScale;
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

	void Rotate(const Quaternion& rotation)
	{
		mRotation = rotation;
		mEuler = rotation.ToEuler();
	}

	void Rotate(const Vector3& degrees)
	{
		InternalDeltaRotate(degrees * DirectX::XM_RADIANS - mEuler);
	}

	void Rotate(float degreesX, float degreesY, float degreesZ)
	{
		InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } * DirectX::XM_RADIANS - mEuler);
	}

	void RotateX(float degreesX)
	{
		InternalDeltaRotate({ degreesX * DirectX::XM_RADIANS - mEuler.x, 0.0f, 0.0f });
	}

	void RotateY(float degreesY)
	{
		InternalDeltaRotate({ 0.0f, degreesY * DirectX::XM_RADIANS - mEuler.y, 0.0f });
	}

	void RotateZ(float degreesZ)
	{
		InternalDeltaRotate({ 0.0f, 0.0f, degreesZ * DirectX::XM_RADIANS - mEuler.z });
	}

	void DeltaRotate(const Quaternion& rotation)
	{
		InternalDeltaRotate(rotation.ToEuler());
	}

	void DeltaRotate(const Vector3& degrees)
	{
		InternalDeltaRotate(degrees * DirectX::XM_RADIANS);
	}

	void DeltaRotate(float degreesX, float degreesY, float degreesZ)
	{
		InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } * DirectX::XM_RADIANS);
	}

	void DeltaRotateX(float degreesX)
	{
		InternalDeltaRotate({ degreesX * DirectX::XM_RADIANS, 0.0f, 0.0f });
	}

	void DeltaRotateY(float degreesY)
	{
		InternalDeltaRotate({ 0.0f, degreesY * DirectX::XM_RADIANS, 0.0f });
	}

	void DeltaRotateZ(float degreesZ)
	{
		InternalDeltaRotate({ 0.0f, 0.0f, degreesZ * DirectX::XM_RADIANS });
	}

	void Scale(const Vector3& scale)
	{
		mScale = scale;
	}

	void Scale(float scale)
	{
		mScale = { scale };
	}

	void ScaleX(float x)
	{
		mScale.x = x;
	}

	void ScaleY(float y)
	{
		mScale.y = y;
	}

	void ScaleZ(float z)
	{
		mScale.z = z;
	}

//***************
// World methods
//***************
	Matrix World() const
	{
		return mParent != nullptr ? Local() * mParent->World() : Local();
	}

	Vector3 WorldForward() const
	{
		return Vector3::Transform(Vector3::UnitZ, WorldRotation());
	}

	Vector3 WorldRight() const
	{
		return Vector3::Transform(Vector3::UnitX, WorldRotation());
	}

	Vector3 WorldUp() const
	{
		return Vector3::Transform(Vector3::UnitY, WorldRotation());
	}

	Vector3 WorldTranslation() const
	{
		return mParent != nullptr ? Translation() + mParent->WorldTranslation() : Translation();
	}

	Quaternion WorldRotation() const
	{
		return mParent != nullptr ? Rotation() * mParent->WorldRotation() : Rotation();
	}

	Vector3 WorldEuler() const
	{
		return mParent != nullptr ? Euler() + mParent->WorldEuler() : Euler();
	}

	Vector3 WorldScaling() const
	{
		return mParent != nullptr ? Scaling() * mParent->WorldScaling() : Scaling();
	}

	void WorldTranslate(const Vector3& translation)
	{
		if (mParent != nullptr)
			Translate(translation - mParent->WorldTranslation());
		else
			Translate(translation);
	}

	void WorldTranslate(float x, float y, float z)
	{
		WorldTranslate({ x, y, z });
	}

	void WorldTranslateX(float x)
	{
		if (mParent != nullptr)
			Translate(x - mParent->WorldTranslation().x, mTranslation.y, mTranslation.z);
		else
			Translate({ x, mTranslation.y, mTranslation.z });
	}

	void WorldTranslateY(float y)
	{
		if (mParent != nullptr)
			Translate(mTranslation.x, y - mParent->WorldTranslation().y, mTranslation.z);
		else
			Translate({ mTranslation.x, y, mTranslation.z });
	}

	void WorldTranslateZ(float z)
	{
		if (mParent != nullptr)
			Translate(mTranslation.x, mTranslation.y, z - mParent->WorldTranslation().z);
		else
			Translate({ mTranslation.x, mTranslation.y, z });
	}

	void WorldRotate(const Quaternion& rotation)
	{
		if (mParent != nullptr)
		{
			Quaternion inverseParentRotation = mParent->WorldRotation();
			inverseParentRotation.Conjugate();
			Rotate(Quaternion::Concatenate(rotation, inverseParentRotation));
		}
		else
			Rotate(rotation);
	}

	void WorldRotate(const Vector3& degrees)
	{
		if (mParent != nullptr)
			Rotate(degrees - mParent->WorldEuler());
		else
			Rotate(degrees);
	}

	void WorldRotate(float degreesX, float degreesY, float degreesZ)
	{
		WorldRotate(Vector3{ degreesX, degreesY, degreesZ });
	}

	void WorldRotateX(float degreesX)
	{
		if (mParent != nullptr)
			RotateX(degreesX - mParent->WorldEuler().x);
		else
			RotateX(degreesX);
	}

	void WorldRotateY(float degreesY)
	{
		if (mParent != nullptr)
			RotateY(degreesY - mParent->WorldEuler().y);
		else
			RotateY(degreesY);
	}

	void WorldRotateZ(float degreesZ)
	{
		if (mParent != nullptr)
			RotateY(degreesZ - mParent->WorldEuler().z);
		else
			RotateY(degreesZ);
	}

	void WorldScale(const Vector3& scale)
	{
		if (mParent != nullptr)
			Scale(scale / mParent->WorldScaling());
		else
			Scale(scale);
	}

	void WorldScale(float scale)
	{
		WorldScale({ scale, scale, scale });
	}

	void WorldScaleX(float x)
	{
		if (mParent != nullptr)
			ScaleX(mScale.x / mParent->WorldScaling().x);
		else
			ScaleX(x);
	}

	void WorldScaleY(float y)
	{
		if (mParent != nullptr)
			ScaleY(mScale.y / mParent->WorldScaling().y);
		else
			ScaleY(y);
	}

	void WorldScaleZ(float z)
	{
		if (mParent != nullptr)
			ScaleZ(mScale.z / mParent->WorldScaling().z);
		else
			ScaleZ(z);
	}

private:
	Transform3* mParent = nullptr;
	Quaternion mRotation = Quaternion::Identity;
	Vector3 mTranslation = Vector3::Zero;
	Vector3 mEuler = Vector3::Zero;
	Vector3 mScale = Vector3::One;

	void InternalDeltaRotate(const Vector3& radians)
	{
		mRotation *= Quaternion::CreateFromYawPitchRoll(radians);
		mEuler += radians;
	}
};
