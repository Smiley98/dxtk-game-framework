#include "pch.h"
#include "BuildingFactory.h"
#include "BuildingRenderer.h"
#include "DebugRenderer.h"
#include "Components.h"

Entity CreateBuilding(Components& components,
	Building::Type type, const BuildingRenderer& renderer)
{
	Entity entity = CreateEntity(components);
	Entity collider = CreateEntity(components);
	AddChild(entity, collider, components);
	components.identifiers.Add(collider).tag = Tags::BUILDING;

	EntityTransform& transform = *components.transforms.Get(collider);
	Collider& capsule = components.colliders.Add(collider);
	capsule.type = Collider::CAPSULE;
	capsule.dynamic = false;

	Vector3 bounds = renderer.Bounds(type);
	switch (type)
	{
		case Building::TD:
		case Building::BMO:
		case Building::PENTA:
		case Building::PINK:
		{
			BoundZ(capsule, bounds);
			//transform.TranslateZ(capsule.hh);
			transform.RotateX(90.0f);
			break;
		}

		// TODO -- add multiple colliders for apartment and office.
		case Building::APARTMENT:
		case Building::CONDO:
		case Building::DUPLEX:
		case Building::OFFICE:
		{
			BoundY(capsule, bounds);
			break;
		}
	}

	Building& building = components.buildings.Add(entity);
	building.durability = renderer.MaxDurability(type);
	building.type = type;

	return entity;
}

std::vector<Entity> CreateMap(Map map, Components& components, const BuildingRenderer& renderer,
	float worldWidth, float worldHeight)
{
	std::vector<Entity> buildings;

	switch (map)
	{
	case LOBSTER_DINNER:
		break;

	case AFTERNOON_DRIVE:
		break;

	case MINTY_AFTERSHAVE:
	{
		const int rows = 4;
		const int cols = 8;
		const float xStep = worldWidth / cols;
		const float yStep = worldHeight / rows;
		float x = xStep * 0.5f;
		float y = yStep * 0.5f;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				Entity building = CreateBuilding(components, Building::TD, renderer);
				components.transforms.Get(building)->Translate(x, y, 0.0f);
				buildings.push_back(building);
				x += xStep;
			}
			x = xStep * 0.5f;
			y += yStep;
		}
	}
		break;
	}

	return buildings;
}
