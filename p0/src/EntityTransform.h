#pragma once
#include "Transform.h"
#include "ComponentHash.h"

struct Components;
class Game;

class EntityTransform : public Transform
{
public:
	static uint32_t Hash();

private:
	friend Game;
	static Components& sComponents;
};
