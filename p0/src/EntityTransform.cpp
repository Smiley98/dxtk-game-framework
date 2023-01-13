#include "EntityTransform.h"
#include "Components.h"

Components* EntityTransform::sComponents = nullptr;

EntityTransform* EntityTransform::Parent() const
{
	Entity parent = sComponents->hierarchies.GetComponent(mSelf)->parent;
	return parent != INVALID_ENTITY ? sComponents->transforms.GetComponent(parent) : nullptr;
}

uint32_t EntityTransform::Hash()
{
	return ComponentHash::TRANSFORM;
}
