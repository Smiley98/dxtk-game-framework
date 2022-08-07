#include "pch.h"
#include "Renderer.h"

void Renderer::Load(std::shared_ptr<DX::DeviceResources> graphics)
{
	auto device = graphics->GetD3DDevice();
	mStates = std::make_unique<DirectX::CommonStates>(device);
	mShader = std::make_shared<DirectX::BasicEffect>(device);
}
