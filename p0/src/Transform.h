#pragma once
#include "SimpleMath.h"

class Transform
{
public:
	Transform();
	~Transform();

	DirectX::SimpleMath::Matrix  World();
	DirectX::SimpleMath::Vector4 Position();

	DirectX::SimpleMath::Vector3 Front();	// local forward
	DirectX::SimpleMath::Vector3 Adjacent();// local right
	DirectX::SimpleMath::Vector3 Above();	// local up

	DirectX::SimpleMath::Vector3 Translation();
	DirectX::SimpleMath::Vector3 Rotation();
	DirectX::SimpleMath::Vector3 Scaling();

	void Translate(const DirectX::SimpleMath::Vector3& translation);
	void Rotate(const DirectX::SimpleMath::Vector3& degreesYXZ);
	void Scale(const DirectX::SimpleMath::Vector3& scaling);

	void DeltaTranslate(const DirectX::SimpleMath::Vector3& translation);
	void DeltaRotate(const DirectX::SimpleMath::Vector3& degreesYXZ);
	void DeltaScale(const DirectX::SimpleMath::Vector3& scaling);

	void Translate(float x, float y);
	void Rotate(float degreesZ);
	void Scale(float scaling);

	void DeltaTranslate(float x, float y);
	void DeltaRotate(float degreesZ);
	void DeltaScale(float scaling);

private:
	DirectX::SimpleMath::Vector3 mPadding;
	DirectX::SimpleMath::Vector3 mTranslation;
	DirectX::SimpleMath::Vector3 mRotation;
	DirectX::SimpleMath::Vector3 mScaling;
};
