#pragma once
#include "DeviceResources.h"
#include "CollisionMath.h"
#include <memory>

namespace Debug
{
	void DrawSphere(const Vector3& position, float radius,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false, bool immediate = false);

	void DrawCapsule(const Vector3& position, const Vector3& direction, float radius, float halfHeight,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false, bool immediate = false);

	void DrawLine(const Vector3& a, const Vector3& b, float thickness,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool immediate = false);

	void DrawFoV(const Vector3& position, const Vector3& direction, const Vector3& target, float length, float degrees,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics, bool immediate = false);

	void DrawDeferred(const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics);

	enum Geometry {
		BOX,
		SPHERE,
		CYLINDER,
		CONE,
		TORUS,
		TEAPOT
	};

	void DrawPrimitive(Geometry geometry, const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);
};
