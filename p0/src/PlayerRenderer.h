#pragma once
#include "Renderer.h"
#include <wrl/client.h>

class PlayerRenderer :
	public Renderer
{
public:
	void Load(std::shared_ptr<DX::DeviceResources> graphics) final;
	void Unload() final;

	void Render(const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics) const;

	// Add multiple player types once we actually have more than one type of player xD
	//struct Player
	//{
	//	enum Type
	//	{
	//		VAN,
	//		MECH
	//	};
	//};
	Vector3 Bounds(/*Player::Type type*/) const;

private:
	std::shared_ptr<DirectX::Model> mVan;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTexture;
};
