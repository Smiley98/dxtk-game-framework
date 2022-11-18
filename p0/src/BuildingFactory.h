#pragma once
#include "Entity.h"
#include "Building.h"

struct Components;
class BuildingRenderer;
Entity CreateBuilding(Components& components,
	Building::Type type, const BuildingRenderer& renderer);
