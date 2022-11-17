#include "Entity.h"

Entity CreateEntity()
{
	static Entity entity = 0;
	return ++entity;
}
