#include "pch.h"
#include "BuildingRenderer.h"
#include "DebugRenderer.h"
#include "Components.h"

void BuildingRenderer::Load(std::shared_ptr<DX::DeviceResources> graphics)
{
	Renderer::Load(graphics);

	mShader->EnableDefaultLighting();
	mShader->SetLightEnabled(2, false);
	mShader->SetLightEnabled(1, false);
	mShader->SetLightDirection(0, Vector3{ 0.0f, 0.0f, -1.0f });
	mShader->SetAmbientLightColor(Vector3{ 0.75f });
	
	using namespace DirectX;
	auto device = graphics->GetD3DDevice();
	mModels[Building::TD] = Model::CreateFromVBO(device, L"assets/meshes/td.vbo", mShader);
	mModels[Building::APARTMENT] = Model::CreateFromVBO(device, L"assets/meshes/apartment.vbo", mShader);
	mModels[Building::BMO] = Model::CreateFromVBO(device, L"assets/meshes/bmo.vbo", mShader);
	mModels[Building::CONDO] = Model::CreateFromVBO(device, L"assets/meshes/condo.vbo", mShader);
	mModels[Building::DUPLEX] = Model::CreateFromVBO(device, L"assets/meshes/duplex.vbo", mShader);
	mModels[Building::OFFICE] = Model::CreateFromVBO(device, L"assets/meshes/office.vbo", mShader);
	mModels[Building::PENTA] = Model::CreateFromVBO(device, L"assets/meshes/penta.vbo", mShader);
	mModels[Building::PINK] = Model::CreateFromVBO(device, L"assets/meshes/pink.vbo", mShader);
}

void BuildingRenderer::Unload()
{
	for (size_t i = 0; i < mModels.size(); i++)
		mModels[i].reset();

	Renderer::Unload();
}

void BuildingRenderer::Render(const Building& building,
	const Matrix& world, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics) const
{
	mShader->SetAmbientLightColor(building.color);
	mModels[building.type]->Draw(graphics->GetD3DDeviceContext(), *mStates, world, view, proj);
}

void BuildingRenderer::DebugBuilding(Entity entity, Components& components,
	const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics, bool capsule)
{
	Entity child = *components.hierarchies.GetComponent(entity)->children.begin();
	Collider& collider = *components.colliders.GetComponent(child);
	Building& building = *components.buildings.GetComponent(entity);
	EntityTransform& buildingTransform = *components.transforms.GetComponent(entity);
	EntityTransform& colliderTransform = *components.transforms.GetComponent(child);

	if (capsule)
	{
		Debug::DrawCapsule(colliderTransform.WorldPosition(), colliderTransform.WorldForward(),
			collider.r, collider.hh, view, proj, graphics, DirectX::Colors::White, false, true);
	}
	
	Render(building, buildingTransform.World(), view, proj, graphics);
}

void BuildingRenderer::DebugMap(std::vector<Entity> map, Components& components,
	const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics, bool capsule)
{
	for (Entity building : map)
	{
		DebugBuilding(building, components, view, proj, graphics, capsule);
	}
}

const DirectX::Model& BuildingRenderer::Model(Building::Type type) const
{
	return *mModels[type];
}

Vector3 BuildingRenderer::Bounds(Building::Type type) const
{
	return mModels[type]->meshes.front()->boundingBox.Extents;
}

float BuildingRenderer::MaxDurability(Building::Type type) const
{
	static std::array<float, Building::COUNT> durabilities
	{
	100.0f,	// TD
	100.0f,	// APARTMENT
	100.0f, // BMO
	100.0f, // CONDO
	100.0f, // DUPLEX
	100.0f, // OFFICE
	100.0f, // PINK
	100.0f, // PENTA
	};

	return durabilities[type];
}
