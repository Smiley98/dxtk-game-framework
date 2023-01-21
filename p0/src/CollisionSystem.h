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
	bool IsColliding(Entity a, Entity b, DirectX::SimpleMath::Vector3& mtv, Components& components);
	void Update(Components& components);
}
