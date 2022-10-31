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
		return mTransform.LocalMatrix();
	}

	Matrix World()
	{
		if (mParent != nullptr)
		{
			return mParent->World() * mTransform.LocalMatrix();
		}
		return mTransform.LocalMatrix();
	}

	void TranslateLocal(const Vector3& localTranslation)
	{
		mTransform.Translate(localTranslation);
	}

	void TranslateWorld(const Vector3& worldTranslation)
	{
		if (mParent != nullptr)
		{
			// Determine difference between parent ie if parent at (5, 5) and translation = (3, 3) then
			// localTranslation = (-2, -2) which makes sense because the child should be below left of parent
			Vector3 localTranslation = worldTranslation - mParent->mTransform.Translation();
			mTransform.Translate(localTranslation);
		}

		// Local = world if root (no parent)
		mTransform.Translate(worldTranslation);
	}

	void RotateLocal(float yaw)
	{
		// Find delta
		//mTransform.RotateZ(yaw);
	}

	void RotateWorld(float yaw)
	{
		if (mParent != nullptr)
		{

		}
		mTransform.RotateY(yaw);
	}

private:
	Transform3 mTransform;
	std::string mName;

	GameObject* mParent = nullptr;
	std::unordered_map<std::string, GameObject*> mChildren;
};