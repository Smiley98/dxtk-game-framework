#pragma once

struct Components;
namespace Kinematics
{
	void Update(Components& components, float dt, size_t steps = 1);
}