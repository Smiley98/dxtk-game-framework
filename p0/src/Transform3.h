#pragma once
#include "SimpleMath.h"

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
			return Matrix::CreateFromQuaternion(mOrientation).Backward();
		}

		Vector3 Right()
		{
			return Matrix::CreateFromQuaternion(mOrientation).Right();
		}

		Vector3 Up()
		{
			return Matrix::CreateFromQuaternion(mOrientation).Up();
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

		void SetForward(const Vector3& forward)
		{
			Quaternion::FromToRotation(Vector3::UnitZ, forward, mOrientation);
			mRotation = mOrientation.ToEuler();
		}

		void Translate(const Vector3& translation)
		{
			mTranslation = translation;
		}

		void Translate(float x, float y, float z)
		{
			mTranslation = { x, y, z };
		}

		void Translate(const Vector2& translation)
		{
			mTranslation.x = translation.x;
			mTranslation.y = translation.y;
		}

		void Translate(float x, float y)
		{
			mTranslation.x = x;
			mTranslation.y = y;
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

		void DeltaTranslate(const Vector2& translation)
		{
			mTranslation.x += translation.x;
			mTranslation.y += translation.y;
		}

		void DeltaTranslate(float x, float y)
		{
			mTranslation.x += x;
			mTranslation.y += y;
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
		Vector3 mTranslation = Vector3::Zero;
		Vector3 mRotation = Vector3::Zero;
		Vector3 mScale = Vector3::One;

		Quaternion mOrientation = Quaternion::Identity;

		void InternalDeltaRotate(const Vector3& radians)
		{
			Quaternion q2 = Quaternion::CreateFromYawPitchRoll(radians);
			Quaternion q1 = mOrientation;
			Quaternion::Concatenate(q2, q1, mOrientation);

			mRotation += radians;
			mRotation.x = fmodf(mRotation.x, TWO_PI);
			mRotation.y = fmodf(mRotation.y, TWO_PI);
			mRotation.z = fmodf(mRotation.z, TWO_PI);
		}

		void InternalDeltaRotateX(float radians)
		{
			Quaternion q2 = Quaternion::CreateFromYawPitchRoll(0.0f, radians, 0.0f);
			Quaternion q1 = mOrientation;
			Quaternion::Concatenate(q2, q1, mOrientation);

			mRotation.x += radians;
			mRotation.x = fmodf(mRotation.x, TWO_PI);
		}

		void InternalDeltaRotateY(float radians)
		{
			Quaternion q2 = Quaternion::CreateFromYawPitchRoll(radians, 0.0f, 0.0f);
			Quaternion q1 = mOrientation;
			Quaternion::Concatenate(q2, q1, mOrientation);

			mRotation.y += radians;
			mRotation.y = fmodf(mRotation.y, TWO_PI);
		}

		void InternalDeltaRotateZ(float radians)
		{
			Quaternion q2 = Quaternion::CreateFromYawPitchRoll(0.0f, 0.0f, radians);
			Quaternion q1 = mOrientation;
			Quaternion::Concatenate(q2, q1, mOrientation);

			mRotation.z += radians;
			mRotation.z = fmodf(mRotation.z, TWO_PI);
		}

		void InternalLookRotation(const Vector3& forward, const Vector3& up, Quaternion& result) noexcept
		{
			using namespace DirectX;

			Quaternion q1;
			Quaternion::FromToRotation(Vector3::UnitZ, forward, q1);

			const XMVECTOR C = XMVector3Cross(forward, up);
			if (XMVector3NearEqual(XMVector3LengthSq(C), g_XMZero, g_XMEpsilon))
			{
				// forward and up are co-linear
				result = q1;
				return;
			}

			const XMVECTOR U = XMQuaternionMultiply(q1, Vector3::Up);

			Quaternion q2;
			Quaternion::FromToRotation(U, up, q2);

			XMStoreFloat4(&result, XMQuaternionMultiply(q2, q1));
		}
	};
}

// Architecture:
// Optimizing for speed by storing translation, rotation and scale matrices + dirty flags doesn't make sense because
// it is not worth the 3x memory increase, adds complexity, and isn't as effective as you'd hope since data is being
// transported between float and vector registers.

// Math:
// *Rotation of q1 followed by q2 = q2 * q1*
// *Vector AB = B - A*
// Must maintain both euler and quaternion deltas
// (otherwise objects will "flip" in arbitrary rotations > 180 degrees).

// Cryptic Math:
// Quaternion::LookRotation() uses Vector3::Forward which is NEGATIVE Z.
// Matrix::Forward() is NEGATIVE Z.
// Quaternion::Concatenate() is q1 * q2 but XMQuaternionMultiply() is q2 * q1.
// Basically, half of "SimpleMath" is fipped inside-out for no apparent reason so I've fixed everything to use +z.
