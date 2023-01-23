#pragma once
#include "Entity.h"
#include "Building.h"

struct Components;
class BuildingRenderer;
Entity CreateBuilding(Components& components, Building::Type type, const BuildingRenderer& renderer);

enum Map
{
	LOBSTER_DINNER,
	AFTERNOON_DRIVE,
	MINTY_AFTERSHAVE
};

std::vector<Entity> CreateMap(Map map, Components& components, const BuildingRenderer& renderer,
	float worldWidth, float worldHeight);
