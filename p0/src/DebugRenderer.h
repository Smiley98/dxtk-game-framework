#pragma once
#include "DXTK.h"
#include <memory>

struct Sphere;
struct Capsule;

namespace Debug
{
	enum Geometry {
		BOX,
		SPHERE,
		CYLINDER,
		CONE,
		TORUS,
		TEAPOT
	};

	void Primitive(Geometry geometry, const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void Draw(const Sphere& sphere, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void Draw(const Capsule& capsule, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);
};
