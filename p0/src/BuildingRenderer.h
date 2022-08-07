#pragma once
#include "Renderer.h"
#include "Tags.h"

class BuildingRenderer :
	public Renderer
{
public:
	void Load(std::shared_ptr<DX::DeviceResources> graphics) final;
	void Unload() final;

	// Testing
	void Render(const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics) const;
	
	Vector3 Bounds(Objects::Building type) const;

private:
	std::unique_ptr<DirectX::Model>	mTd;
	// *Insert other building models here*
};
