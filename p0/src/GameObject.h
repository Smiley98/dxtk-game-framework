#pragma once
#include "Transform3.h"

class GameObject
{
public:
	GameObject* const Parent()
	{
		return mParent;
	}

	void SetParent(GameObject* parent)
	{
		if (mParent != nullptr)
			mParent->RemoveChild(mName);

		parent->AddChild(this);
		mParent = parent;
	}

	GameObject* Child(const std::string& name)
	{
		return mChildren.at(name);
	}

	void AddChild(GameObject* child)
	{
		child->mParent = this;
		mChildren.insert({ child->Name(), child });
	}

	void RemoveChild(const std::string& name)
	{
		mChildren.at(name)->mParent = nullptr;
		mChildren.erase(name);
	}

	std::string Name()
	{
		return mName;
	}

	void SetName(const std::string& name)
	{
		if (mParent != nullptr)
			mParent->RemoveChild(mName);
		mName = name;
		if (mParent != nullptr)
			mParent->AddChild(this);
	}

	Matrix Local()
	{
		return mTransform.Local();
	}

	Matrix World()
	{
		if (mParent != nullptr)
		{
			return mTransform.Local() * mParent->World();
		}
		return mTransform.Local();
	}

	Vector3 WorldTranslation()
	{
		if (mParent != nullptr)
		{
			return mTransform.Translation() + mParent->WorldTranslation();
		}
		return mTransform.Translation();
	}

	Quaternion WorldRotation()
	{
		if (mParent != nullptr)
		{
			return mTransform.Rotation() * mParent->WorldRotation();
		}
		return mTransform.Rotation();
	}

	Vector3 WorldScale()
	{
		if (mParent != nullptr)
		{
			return mTransform.Scaling() * mParent->WorldScale();
		}
		return mTransform.Scaling();
	}

	void TranslateLocal(const Vector3& localTranslation)
	{
		mTransform.Translate(localTranslation);
	}

	void TranslateWorld(const Vector3& worldTranslation)
	{
		using namespace DirectX;
		if (mParent != nullptr)
		{
			// Compute local translation based on world translation relative to the parent.
			Vector3 localTranslation = worldTranslation - mParent->WorldTranslation();
			TranslateLocal(localTranslation);
		}
		else
		{
			// Local = world if there's no parent.
			TranslateLocal(worldTranslation);
		}
	}

	void RotateLocal(const Quaternion& localRotation)
	{
		mTransform.Rotate(localRotation);
	}

	void RotateWorld(const Quaternion& worldRotation)
	{
		if (mParent != nullptr)
		{
			// Compute local rotation based on world orientation relative to the parent.
			Quaternion inverseParentRotation = mParent->WorldRotation();
			inverseParentRotation.Conjugate();
			RotateLocal(Quaternion::Concatenate(worldRotation, inverseParentRotation));
		}
		else
		{
			// Local = world if there's no parent.
			RotateLocal(worldRotation);
		}
	}

	void RotateLocal(const Vector3& localRotation)
	{
		mTransform.Rotate(localRotation);
	}

	void ScaleLocal(const Vector3& localScale)
	{
		mTransform.Scale(localScale);
	}

	void ScaleWorld(const Vector3& worldScale)
	{

		if (mParent != nullptr)
		{
			Vector3 localScale = worldScale / mParent->WorldScale();
			ScaleLocal(localScale);
		}
		else
		{
			ScaleLocal(worldScale);
		}
	}

private:
	Transform3 mTransform;
	std::string mName;

	GameObject* mParent = nullptr;
	std::unordered_map<std::string, GameObject*> mChildren;

	// Using this for forward kinematics is lazy since we only need ONE of translation, orientation or scale at a time!
	//void Decompose(Vector3& translation, Quaternion& orientation, Vector3& scale, const Matrix& world)
	//{
	//	Matrix world = mParent->World();
	//	XMVECTOR t, r, s;
	//	XMMatrixDecompose(&s, &r, &t, world);
	//
	//	translation = t;
	//	orientation = r;
	//	scale = s;
	//}
};
