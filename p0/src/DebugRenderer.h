#pragma once
#include "DXTK.h"
#include "Geometry.h"
#include <memory>

namespace Collision
{
	class SphereCollider;
	class CapsuleCollider;
}

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

	void Line(const Vector3& a, const Vector3& b, float thickness,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White);

	void Sphere(const Vector3& position, float radius,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void Capsule(const Transform3& transform, float radius, float halfHeight,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void InRange(const Transform3& viewer, const Vector3& target, float length, float fov, /*(degrees)*/
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics);
};
