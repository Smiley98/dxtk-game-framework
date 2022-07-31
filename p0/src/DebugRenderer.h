#pragma once
#include "DXTK.h"
#include "DeviceResources.h"
#include "Collision.h"
#include <memory>

namespace Debug
{
	enum Type {
		BOX,
		SPHERE,
		CYLINDER,
		CONE,
		TORUS,
		TEAPOT
	};

	void Primitive(Type geometry,
		const DirectX::SimpleMath::Matrix& world,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color = DirectX::Colors::White);

	void Draw(const Collision::SphereCollider& collider, std::shared_ptr<DX::DeviceResources> graphics);
	void Draw(const Collision::CapsuleCollider& collider, std::shared_ptr<DX::DeviceResources> graphics);
};

