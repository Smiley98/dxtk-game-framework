#include "pch.h"
#include "Map.h"
#include "DebugRenderer.h"
#include "Utility.h"

// Collider half height is either y or z depending on the building, hence the need for this switch.
/*
	float radius = bounds.x;
	float halfHeight = bounds.y - radius;
	switch (type)
	{
		case Building::TD:
		case Building::BMO:
		case Building::CONDO:
		case Building::OFFICE:
		case Building::PENTA:
		case Building::PINK:
			halfHeight = bounds.z - radius;
			transform.Translate({ 0.0f, 0.0f, halfHeight });
			transform.RotateX(90.0f);
			break;

		case Building::APARTMENT:
		case Building::DUPLEX:
			break;
	}
*/
