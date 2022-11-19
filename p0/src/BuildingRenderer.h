#pragma once
#include "Renderer.h"
#include "Building.h"
#include <array>

class BuildingRenderer :
	public Renderer
{
public:
	void Load(std::shared_ptr<DX::DeviceResources> graphics) final;
	void Unload() final;

	void Render(const Building& building,
		const Matrix& world, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics) const;

	const DirectX::Model& Model(Building::Type type) const;
	Vector3 Bounds(Building::Type type) const;
	float MaxDurability(Building::Type type) const;

private:
	std::array<std::unique_ptr<DirectX::Model>, Building::COUNT> mModels;
};