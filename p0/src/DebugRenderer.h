#pragma once
#include "DXTK.h"
#include "Geometry.h"
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

	void Capsule(const Transform& transform, float radius, float halfHeight,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void InRange(const Transform& viewer, const Vector3& target, float length, float fov, /*(degrees)*/
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics);
};
