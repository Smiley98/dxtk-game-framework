#pragma once
#include "Buildings.h"
#include <array>

struct BuildingId
{
	Id id;
	Building::Type type;
};

// Hard-code Minty Aftershave first before making any file tools.
// No sense in making some super-optimized binary thing when you're not even sure if the original text files work.
// What I mean by this is the perspective or positions might not line up so all buildings will have incorrect positions.
// At which point I'll either need to redo blender levels, or represent things entirely in code.
class Map
{
public:
// The Colliders object should reside in the scene since it needs to be shared between players, projectiles, and buildings.
	//BuildingId Add(Building::Type type, Collision::Colliders& colliders);
	BuildingId Add(Building::Type type, Collision2::Colliders& colliders);
	void Remove(BuildingId building);
	Building* Get(BuildingId building);

	void Render(const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics);

	// These may not need to be virtual -- common code for resetting durabilities, and positions if from file.
	//virtual void Load() = 0;
	//virtual void Unload() = 0;

private:
	std::array<UnorderedVector<Building>, Building::COUNT> mBuildings;
};

class MintyAftershave :
	public Map
{
public:
	void Reset();
};
