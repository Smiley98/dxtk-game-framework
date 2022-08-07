#pragma once
#include "Renderer.h"
#include "Tags.h"

class PlayerRenderer :
	public Renderer
{
public:
	void Load(std::shared_ptr<DX::DeviceResources> graphics) final;
	void Unload() final;

	// Testing
	void Render(const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics) const;

	Vector3 Bounds(Objects::Player type) const;

private:
	std::shared_ptr<DirectX::Model> mVan;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
};
