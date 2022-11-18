#pragma once
#include "Entity.h"
#include "Building.h"

struct Components;
Entity CreateBuilding(Components& components, float radius, float halfHeight, Building::Type type);
