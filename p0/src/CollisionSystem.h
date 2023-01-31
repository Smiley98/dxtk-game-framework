#pragma once
#include "Entity.h"

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
	}
}

struct Components;
namespace Collision
{
	void Update(Components& components);
	bool IsColliding(Entity a, Entity b, Vector3& mtv, Components& components);
}
