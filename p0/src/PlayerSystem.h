#pragma once
#include "Input.h"

struct Components;
namespace Players
{
	void Update(Components& components, const DX::Input& input, float dt);
}
