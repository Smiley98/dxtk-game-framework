#pragma once
#include "Component.h"
#include "Collision2.h"

Entity CreatePlayer(
	ComponentCollection<Transform3>& transforms,
	ComponentCollection<Collision2::CapsuleCollider>& capsules
)
{
	Entity entity = CreateEntity();
	Transform3& transform = transforms.Add(entity);
	Collision2::CapsuleCollider& capsule = capsules.Add(entity);
	capsule.transform = &transform;
	return entity;
}
