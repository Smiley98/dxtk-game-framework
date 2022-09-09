#pragma once
#include "Renderer.h"

class MiscRenderer :
	public Renderer
{
public:
	void Load(std::shared_ptr<DX::DeviceResources> graphics) final;
	void Unload() final;

	void Cone(const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics) const;

private:
	std::shared_ptr<DirectX::Model> mCone;
};