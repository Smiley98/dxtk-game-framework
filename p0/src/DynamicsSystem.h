#pragma once

struct Components;
namespace Dynamics
{
	void Update(Components& components, float dt, size_t steps = 1);
}