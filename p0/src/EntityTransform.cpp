#include "EntityTransform.h"
#include "ComponentMap.h"

ComponentMap<EntityTransform>* EntityTransform::sTransforms = nullptr;

EntityTransform* EntityTransform::Parent() const
{
	return parent != INVALID_ENTITY ? sTransforms->GetComponent(parent) : nullptr;
}

uint32_t EntityTransform::Hash()
{
	return ComponentHash::TRANSFORM;
}
