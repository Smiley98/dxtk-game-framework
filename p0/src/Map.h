#pragma once
#include "Building.h"
#include <vector>

// This class serves no purpose until we develop a file loading mechanism.
// No need to use inheritance. All building loading works the same so just have multiple Map objects (1 per file).
// Hard-coding Minty Aftershave in scene for now.
class Map
{
public:
	//void Load();
	//void Unload();
	//void Render(const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics);
private:
	std::vector<Building> mBuildings;
};
