#include "pch.h"
#include "MiscRenderer.h"

using namespace DirectX;

void MiscRenderer::Load(std::shared_ptr<DX::DeviceResources> graphics)
{
	Renderer::Load(graphics);
	auto device = graphics->GetD3DDevice();

	mShader = std::make_shared<BasicEffect>(device);
	mShader->EnableDefaultLighting();

	mCone = Model::CreateFromVBO(device, L"assets/meshes/cone.vbo", mShader);
}

void MiscRenderer::Unload()
{
	mCone.reset();
	Renderer::Unload();
}

void MiscRenderer::Cone(const Matrix& world, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics) const
{
	mCone->Draw(graphics->GetD3DDeviceContext(), *mStates, world, view, proj);
}