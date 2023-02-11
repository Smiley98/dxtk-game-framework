#pragma once
#include "Renderer.h"

class MiscRenderer :
	public Renderer
{
public:
	void Load(std::shared_ptr<DX::DeviceResources> graphics) final;
	void Unload() final;

	void Text(const Vector3& position, const std::wstring& text, DirectX::XMVECTOR color = DirectX::Colors::White);

	void Cone(const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics) const;

	// *TRIANGLES MUST HAVE CLOCKWISE WINDING ORDER*
	void Triangle(const Vector3& p0 /*top*/, const Vector3& p1 /*right*/, const Vector3& p2 /*left*/,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics) const;

private:
	std::unique_ptr<DirectX::SpriteBatch> mSprites;
	std::unique_ptr<DirectX::SpriteFont> mFont;
	std::shared_ptr<DirectX::Model> mCone;
};
