#include "pch.h"
#include "PlayerRenderer.h"
#include "DebugRenderer.h"
#include "Components.h"

using namespace DirectX;

void PlayerRenderer::Load(std::shared_ptr<DX::DeviceResources> graphics)
{
	Renderer::Load(graphics);
	auto device = graphics->GetD3DDevice();

	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/van.dds", nullptr, mTexture.ReleaseAndGetAddressOf()));
	mShader = std::make_shared<BasicEffect>(device);
	mShader->EnableDefaultLighting();
	mShader->SetLightDirection(0, { 0.0f, 0.0f, -1.0f });
	mShader->SetAmbientLightColor(Vector3::One * 0.5f);
	mShader->SetDiffuseColor(Vector3::One * 0.75f);
	mShader->SetSpecularColor(Vector3::One * 0.1f);
	mShader->SetSpecularPower(256.0f);
	mShader->SetTextureEnabled(true);
	mShader->SetTexture(mTexture.Get());
	mVan = Model::CreateFromVBO(device, L"assets/meshes/vanY.vbo", mShader);
}

void PlayerRenderer::Unload()
{
	mTexture.Reset();
	mVan.reset();
	Renderer::Unload();
}

void PlayerRenderer::Render(const Matrix& world, const Matrix& view, const Matrix& proj,
	std::shared_ptr<DX::DeviceResources> graphics) const
{
	mVan->Draw(graphics->GetD3DDeviceContext(), *mStates, world, view, proj);
}

void PlayerRenderer::DebugPlayer(Entity entity, Components& components,
	const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics, bool capsule)
{
	EntityTransform& playerTransform = *components.transforms.GetComponent(entity);
	Collider& playerCollider = *components.colliders.GetComponent(entity);
	if (capsule)
	{
		Debug::Capsule(playerTransform.WorldPosition(), playerTransform.WorldForward(),
			playerCollider.r, playerCollider.hh, view, proj, graphics);
	}
	Render(playerTransform.World(), view, proj, graphics);
}

Vector3 PlayerRenderer::Bounds(/*Player::Type type*/) const
{
	return mVan->meshes.front()->boundingBox.Extents;
	//switch (type)
	//{
	//case Player::VAN:
	//	return mVan->meshes.front()->boundingBox.Extents;
	//case Player::MECH:
	//	return {};
	//default:
	//	return {};
	//}
}
