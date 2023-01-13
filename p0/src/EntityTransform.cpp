#include "EntityTransform.h"
#include "Components.h"

EntityTransform* EntityTransform::Parent() const
{
	return parent != INVALID_ENTITY ? sComponents.transforms.GetComponent(parent) : nullptr;
}

uint32_t EntityTransform::Hash()
{
	return ComponentHash::TRANSFORM;
}
