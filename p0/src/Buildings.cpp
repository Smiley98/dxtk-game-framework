#include "pch.h"
#include "Buildings.h"
#include "Tags.h"
#include <array>

using namespace DirectX;

std::unique_ptr<DirectX::CommonStates> Building::sStates;
std::shared_ptr<DirectX::BasicEffect> Building::sShader;
std::array<std::unique_ptr<DirectX::Model>, Building::COUNT> Building::sModels;
std::array<float, Building::COUNT> Building::sDurabilities {
	100.0f,	// TD
	100.0f,	// APARTMENT
	100.0f, // BMO
	100.0f, // CONDO
	100.0f, // DUPLEX
	100.0f, // OFFICE
	100.0f, // PINK
	100.0f, // PENTA
};

void Building::Load(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto device = graphics->GetD3DDevice();
	sStates = std::make_unique<CommonStates>(device);
	sShader = std::make_shared<BasicEffect>(device);

	// DXTK BasicEffect constructor source:
	/*
	lightingEnabled(false),
    preferPerPixelLighting(false),
    vertexColorEnabled(false),
    textureEnabled(false),
    biasedVertexNormals(false)
	*/
	
	sShader->EnableDefaultLighting();
	sShader->SetLightEnabled(2, false);
	sShader->SetLightEnabled(1, false);
	sShader->SetLightDirection(0, Vector3{ 0.0f, 0.0f, -1.0f });
	sShader->SetAmbientLightColor(Vector3{ 0.75f });

	sModels[TD] = Model::CreateFromVBO(device, L"assets/meshes/td.vbo", sShader);
	sModels[APARTMENT] = Model::CreateFromVBO(device, L"assets/meshes/apartment.vbo", sShader);
	sModels[BMO] = Model::CreateFromVBO(device, L"assets/meshes/bmo.vbo", sShader);
	sModels[CONDO] = Model::CreateFromVBO(device, L"assets/meshes/condo.vbo", sShader);
	sModels[DUPLEX] = Model::CreateFromVBO(device, L"assets/meshes/duplex.vbo", sShader);
	sModels[OFFICE] = Model::CreateFromVBO(device, L"assets/meshes/office.vbo", sShader);
	sModels[PENTA] = Model::CreateFromVBO(device, L"assets/meshes/penta.vbo", sShader);
	sModels[PINK] = Model::CreateFromVBO(device, L"assets/meshes/pink.vbo", sShader);
}

void Building::Unload()
{
	for (size_t i = 0; i < sModels.size(); i++)
	{
		sModels[i].reset();
	}
	sShader.reset();
	sStates.reset();
}

void Building::Draw(const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
{
	sShader->SetAmbientLightColor(color);
	sModels[type]->Draw(graphics->GetD3DDeviceContext(), *sStates, transform.World(), view, proj);
}

void Building::Draw(const Building& building, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
{
	// Placeholder for now. Gonna need custom shaders if we want to colour the entire building with a single variable...
	sShader->SetAmbientLightColor(building.color);
	//sModels[building.type]->Draw(graphics->GetD3DDeviceContext(), *sStates, Matrix::CreateTranslation(building.position), view, proj);
	sModels[building.type]->Draw(graphics->GetD3DDeviceContext(), *sStates, building.transform.World(), view, proj);
}

const DirectX::Model& Building::Model(Type type)
{
	return *sModels[type];
}

Vector3 Building::Bounds(Type type)
{
	return sModels[type]->meshes.front()->boundingBox.Extents;
}

float Building::Durability(Type type)
{
	return sDurabilities[type];
}
