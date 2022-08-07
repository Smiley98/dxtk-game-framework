#include "pch.h"
#include "BuildingRenderer.h"

using namespace DirectX;

void BuildingRenderer::Load(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto device = graphics->GetD3DDevice();

	Renderer::Load(graphics);
	mShader = std::make_shared<BasicEffect>(device);
	mShader->EnableDefaultLighting();
	mShader->SetLightDirection(0, { 0.0f, 0.0f, -1.0f });
	mShader->SetAmbientLightColor(Vector3::One * 0.5f);
	mShader->SetDiffuseColor(Vector3::One * 0.75f);
	mShader->SetSpecularColor(Vector3::One * 0.1f);
	mShader->SetSpecularPower(1024.0f);
	mShader->SetTextureEnabled(false);

	mTd = Model::CreateFromVBO(device, L"assets/meshes/td.vbo", mShader);
}

void BuildingRenderer::Unload()
{
	mTd.reset();
}

void BuildingRenderer::Render(const Matrix& world, const Matrix& view, const Matrix& proj,
	std::shared_ptr<DX::DeviceResources> graphics) const
{
	mTd->Draw(graphics->GetD3DDeviceContext(), *mStates, world, view, proj);
}

Vector3 BuildingRenderer::Bounds(Objects::Building type) const
{
	switch (type)
	{
	case Objects::TD:
		return mTd->meshes.front()->boundingBox.Extents;
	case Objects::CONDO:
		return {};
	}
}
