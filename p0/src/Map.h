#pragma once
#include "Buildings.h"
#include <array>

struct BuildingId
{
	Id id;
	Buildings::Type type;
};

// Hard-code Minty Aftershave first before making any file tools.
// No sense in making some super-optimized binary thing when you're not even sure if the original text files work.
// What I mean by this is the perspective or positions might not line up so all buildings will have incorrect positions.
// At which point I'll either need to redo blender levels, or represent things entirely in code.
class Map
{
public:
// The Colliders object shguld reside in the scene since it needs to be shared between players, projectiles, and buildings.
	BuildingId Add(Buildings::Type type, Collision::Colliders& colliders);
	void Remove(BuildingId building);
	Buildings::Building* Get(BuildingId building);

	virtual void Load() = 0;
	virtual void Unload() = 0;

private:
	std::array<UnorderedVector<Buildings::Building>, Buildings::COUNT> mBuildings;
};

class MintyAftershave :
	public Map
{
public:
	virtual void Load() override;
	virtual void Unload() override;
};