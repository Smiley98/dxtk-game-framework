#pragma once
#include "Transform.h"
#include "ComponentHash.h"
#include "Entity.h"

template<typename T>
class ComponentMap;
class Game;

class EntityTransform : public Transform
{
public:
	static uint32_t Hash();

	// Remember to assign this back to INVALID_ENTITY on parent destruction.
	Entity parent = INVALID_ENTITY;

	//***************
	// World getters
	//***************
	Matrix World() const
	{
		return Parent() != nullptr ? Local() * Parent()->World() : Local();
	}

	Vector3 WorldForward() const
	{
#if Y_FORWARD
		return Vector3::Transform(Vector3::UnitY, WorldRotation());
#else
		return Vector3::Transform(Vector3::UnitZ, WorldRotation());
#endif
	}

	Vector3 WorldRight() const
	{
		return Vector3::Transform(Vector3::UnitX, WorldRotation());
	}

	Vector3 WorldUp() const
	{
#if Y_FORWARD
		return Vector3::Transform(Vector3::UnitZ, WorldRotation());
#else
		return Vector3::Transform(Vector3::UnitY, WorldRotation());
#endif
	}

	Vector3 WorldTranslation() const
	{
		return Parent() != nullptr ? Translation() + Parent()->WorldTranslation() : Translation();
	}

	Quaternion WorldRotation() const
	{
		return Parent() != nullptr ? Rotation() * Parent()->WorldRotation() : Rotation();
	}

	Vector3 WorldEuler() const
	{
		return Parent() != nullptr ? Euler() + Parent()->WorldEuler() : Euler();
	}

	Vector3 WorldScaling() const
	{
		return Parent() != nullptr ? Scaling() * Parent()->WorldScaling() : Scaling();
	}

private:
	EntityTransform* Parent() const;

	friend Game;
	static ComponentMap<EntityTransform>* sTransforms;
};

//Transform* const Parent() const
//{
//	return mParent;
//}
//
//void SetParent(Transform* parent)
//{
//	mParent = parent;
//}

// World setters are cursed. For example, if we call WorldTranslate(0.0f, 0.0f, 0.0f) the object will have a
// translation of 0, but if its parent translates then the child's translation will be overwritten.
// Moreover, rotations make translations hard to reason about. If a child has a translation of 0 but its parent is
// rotated 10 degrees, the child will not be at world 0 since *position* is not the same as *translation*.
// TLDR unparent an object if you want to work in world coordinates.
/*
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
*/
