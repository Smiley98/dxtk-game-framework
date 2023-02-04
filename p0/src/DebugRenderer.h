#pragma once
#include "DeviceResources.h"
#include "CollisionMath.h"
#include <memory>

namespace Debug
{
	void DrawSphere(const Vector3& position, float radius,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void DrawCapsule(const Vector3& position, const Vector3& direction, float radius, float halfHeight,
		DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);

	void DrawLine(const Vector3& a, const Vector3& b, float thickness = 10.0f,
		DirectX::XMVECTOR color = DirectX::Colors::White);

	void DrawFoV(const Vector3& position, const Vector3& direction, const Vector3& target,
		float length, float degrees, float thickness = 10.0f,
		DirectX::XMVECTOR inColor = DirectX::Colors::Green, DirectX::XMVECTOR outColor = DirectX::Colors::Red);

	// All of the above functions store their parameters and render during this call
	void DrawDeferred(const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics);

	enum Primitive {
		BOX,
		SPHERE,
		CYLINDER,
		CONE,
		TORUS,
		TEAPOT
	};

	// This is immediate
	void Draw(Primitive primitive, const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color = DirectX::Colors::White, bool wireframe = false);
};
