#pragma once
#include "DXTK.h"
#include <memory>

struct SphereCollider;
struct CapsuleCollider;

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

	void Draw(const SphereCollider& collider, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void Draw(const CapsuleCollider& collider, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);
};
