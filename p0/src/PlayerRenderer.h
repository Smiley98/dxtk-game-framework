#pragma once
#include "Renderer.h"
#include <wrl/client.h>

struct Components;
class PlayerRenderer :
	public Renderer
{
public:
	void Load(std::shared_ptr<DX::DeviceResources> graphics) final;
	void Unload() final;

	void Render(const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics) const;

	void DebugPlayer(Entity entity, Components& components,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics, bool capsule = false);

	Vector3 Bounds(/*Player::Type type*/) const;

private:
	std::shared_ptr<DirectX::Model> mVan;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
};
