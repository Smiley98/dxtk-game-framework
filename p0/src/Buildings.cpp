#include "pch.h"
#include "Buildings.h"
#include "Tags.h"
#include <array>

using namespace DirectX;

std::unique_ptr<DirectX::CommonStates> Buildings::sStates;
std::shared_ptr<DirectX::BasicEffect> Buildings::sShader;
std::array<std::unique_ptr<DirectX::Model>, Buildings::COUNT> Buildings::sModels;
std::array<float, Buildings::COUNT> Buildings::sDurabilities {
	100.0f,	// TD
	100.0f,	// APARTMENT
	100.0f, // BMO
	100.0f, // CONDO
	100.0f, // DUPLEX
	100.0f, // OFFICE
	100.0f, // PINK
	100.0f, // PENTA
};

void Buildings::Load(std::shared_ptr<DX::DeviceResources> graphics)
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

void Buildings::Unload()
{
	for (size_t i = 0; i < sModels.size(); i++)
	{
		sModels[i].reset();
	}
	sShader.reset();
	sStates.reset();
}

void Buildings::Draw(const Building& building, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
{
	sModels[building.type]->Draw(graphics->GetD3DDeviceContext(), *sStates, Matrix::CreateTranslation(building.position), view, proj);
}

const DirectX::Model& Buildings::Model(Type type)
{
	return *sModels[type];
}

Vector3 Buildings::Bounds(Type type)
{
	return sModels[type]->meshes.front()->boundingBox.Extents;
}

float Buildings::Durability(Type type)
{
	return sDurabilities[type];
}
