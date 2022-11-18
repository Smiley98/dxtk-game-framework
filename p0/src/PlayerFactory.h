#pragma once
#include "Entity.h"
#include "Tags.h"

template<typename Component>
class ComponentCollection;

class Transform3;
namespace Collision2
{
	struct CapsuleCollider;
}

Entity CreatePlayer(
	ComponentCollection<Tags::Tag>& tags,
	ComponentCollection<Transform3>& transforms,
	ComponentCollection<Collision2::CapsuleCollider>& capsules,
	float radius, float halfHeight
);
