#pragma once
#include "Entity.h"

struct Components;
class PlayerRenderer;
Entity CreatePlayer(Components& components, const PlayerRenderer& renderer);
