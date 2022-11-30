#pragma once

struct Components;
namespace Kinematics
{
	void Update(Components& components, float dt, float timeSteps = 1.0f);
}