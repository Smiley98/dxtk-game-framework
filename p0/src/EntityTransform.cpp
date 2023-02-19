#include "EntityTransform.h"
#include "Components.h"

Components* EntityTransform::sComponents = nullptr;

EntityTransform* EntityTransform::Parent() const
{
	Entity parent = sComponents->hierarchies.Get(mSelf)->parent;
	return parent != INVALID_ENTITY ? sComponents->transforms.Get(parent) : nullptr;
}

uint32_t EntityTransform::Hash()
{
	return ComponentHash::TRANSFORM;
}
