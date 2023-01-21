#pragma once
#include "DeviceResources.h"
#include "CollisionMath.h"
#include <memory>

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

	void Primitive(Geometry geometry, const Matrix& world,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void Line(const Vector3& a, const Vector3& b, float thickness,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White);

	void Sphere(const Vector3& position, float radius,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void Capsule(const Vector3& position, const Vector3& direction, float radius, float halfHeight,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void InRange(
		const Vector3& position, const Vector3& direction,
		const Vector3& target, float length, float fov, /*(degrees)*/
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics);
};
