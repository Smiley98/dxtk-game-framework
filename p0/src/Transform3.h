#pragma once
#include "SimpleMath.h"
#include "Utility.h"

namespace
{
	using namespace DirectX::SimpleMath;
	constexpr float DEGREES = 180.0f / DirectX::XM_PI;
	constexpr float RADIANS = DirectX::XM_PI / 180.0f;
	constexpr float TWO_PI = DirectX::XM_2PI;

	class Transform3
	{
	public:

		Matrix LocalMatrix()
		{
			return Matrix::CreateScale(mScale) *
				Matrix::CreateFromQuaternion(mOrientation) *
				Matrix::CreateTranslation(mTranslation);
		}

		Vector3 Forward()
		{
			return Vector3::Transform(Vector3::UnitZ, mOrientation);
		}

		Vector3 Right()
		{
			return Vector3::Transform(Vector3::UnitX, mOrientation);
		}

		Vector3 Up()
		{
			return Vector3::Transform(Vector3::UnitY, mOrientation);
		}

		Vector3 Translation()
		{
			return mTranslation;
		}

		Vector3 Rotation()
		{
			return mRotation * DEGREES;
		}

		Quaternion Orientation()
		{
			return mOrientation;
		}

		Vector3 Scaling()
		{
			return mScale;
		}

		void Orientate(const Quaternion& orientation)
		{
			mOrientation = orientation;
			mRotation = orientation.ToEuler();
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

		void Rotate(const Vector3& degrees)
		{
			InternalDeltaRotate(degrees * RADIANS - mRotation);
		}

		void Rotate(float degreesX, float degreesY, float degreesZ)
		{
			InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } * RADIANS - mRotation);
		}

		void RotateX(float degrees)
		{
			float x = degrees * RADIANS;
			float dx = x - mRotation.x;
			InternalDeltaRotateX(dx);
		}

		void RotateY(float degrees)
		{
			float y = degrees * RADIANS;
			float dy = y - mRotation.y;
			InternalDeltaRotateY(dy);
		}

		void RotateZ(float degrees)
		{
			float z = degrees * RADIANS;
			float dz = z - mRotation.z;
			InternalDeltaRotateZ(dz);
		}

		void DeltaRotate(const Vector3& degrees)
		{
			InternalDeltaRotate(degrees * RADIANS);
		}

		void DeltaRotate(float degreesX, float degreesY, float degreesZ)
		{
			InternalDeltaRotate(Vector3{ degreesX, degreesY, degreesZ } * RADIANS);
		}

		void DeltaRotateX(float degrees)
		{
			float x = degrees * RADIANS;
			InternalDeltaRotateX(x);
		}

		void DeltaRotateY(float degrees)
		{
			float y = degrees * RADIANS;
			InternalDeltaRotateY(y);
		}

		void DeltaRotateZ(float degrees)
		{
			float z = degrees * RADIANS;
			InternalDeltaRotateZ(z);
		}

		void Scale(const Vector3& scale)
		{
			mScale = scale;
		}

		void Scale(float scale)
		{
			mScale = { scale };
		}

	private:
		// TODO -- since we have some room left, we should implement parenting here since we only need parent* and no children[].
		Vector3 mTranslation = Vector3::Zero;
		Vector3 mRotation = Vector3::Zero;
		Vector3 mScale = Vector3::One;

		Quaternion mOrientation = Quaternion::Identity;

		void InternalDeltaRotate(const Vector3& radians)
		{
			mOrientation *= Quaternion::CreateFromYawPitchRoll(radians);
			mRotation += radians;
			mRotation.x = fmodf(mRotation.x, TWO_PI);
			mRotation.y = fmodf(mRotation.y, TWO_PI);
			mRotation.z = fmodf(mRotation.z, TWO_PI);
		}

		void InternalDeltaRotateX(float radians)
		{
			mOrientation *= Quaternion::CreateFromYawPitchRoll(0.0f, radians, 0.0f);
			mRotation.x += radians;
			mRotation.x = fmodf(mRotation.x, TWO_PI);
		}

		void InternalDeltaRotateY(float radians)
		{
			mOrientation *= Quaternion::CreateFromYawPitchRoll(radians, 0.0f, 0.0f);
			mRotation.y += radians;
			mRotation.y = fmodf(mRotation.y, TWO_PI);
		}

		void InternalDeltaRotateZ(float radians)
		{
			mOrientation *= Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, radians);
			mRotation.z += radians;
			mRotation.z = fmodf(mRotation.z, TWO_PI);
		}
	};
}

// Architecture:
// Optimizing for speed by storing translation, rotation and scale matrices + dirty flags doesn't make sense because
// it is not worth the 3x memory increase, adds complexity, and isn't as effective as you'd hope since data is being
// transported between float and vector registers.

// Math:
// Rotation of q1 followed by q2 = q2 * q1
// Concatenate(q1, q2) is equivalent to q2 * q1
// Must maintain both euler and quaternion deltas otherwise objects will "flip" in arbitrary rotations > 180 degrees!

// Unity's solution that should work but doesn't (even after changing internal Vector3::Forward to Vector3::UnitZ).
//void SetForward(const Vector3& forward)
//{
//	mOrientation = Quaternion::LookRotation(forward, Vector3::Up);
//	mRotation = mOrientation.ToEuler();
//}

// My jank solution that works for pitch and yaw.
//void SetForward(const Vector3& forward)
//{
//	Vector3 right = forward.Cross(Vector3::Up);
//	Vector3 up = forward.Cross(right);
//	Matrix rotation{ -right, -up, forward };
//	InternalDeltaRotate(rotation.ToEuler() - mRotation);
//}
