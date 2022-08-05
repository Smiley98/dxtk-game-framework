#include "pch.h"
#include "DebugRenderer.h"
#include "Collision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Debug
{
	void Primitive(Geometry geometry, const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color, bool wireframe)
	{
		auto context = graphics->GetD3DDeviceContext();
		switch (geometry)
		{
		case Debug::BOX:
			GeometricPrimitive::CreateCube(context)->Draw(world, view, proj, color, nullptr, wireframe);
			break;
		case Debug::SPHERE:
			GeometricPrimitive::CreateSphere(context)->Draw(world, view, proj, color, nullptr, wireframe);
			break;
		case Debug::CYLINDER:
			GeometricPrimitive::CreateCylinder(context)->Draw(world, view, proj, color, nullptr, wireframe);
			break;
		case Debug::CONE:
			GeometricPrimitive::CreateCone(context)->Draw(world, view, proj, color, nullptr, wireframe);
			break;
		case Debug::TORUS:
			GeometricPrimitive::CreateTorus(context)->Draw(world, view, proj, color, nullptr, wireframe);
			break;
		case Debug::TEAPOT:
			GeometricPrimitive::CreateTeapot(context)->Draw(world, view, proj, color, nullptr, wireframe);
			break;
		default:
			break;
		}
	}

	void Draw(const Sphere& sphere, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color, bool wireframe)
	{
		auto shape = GeometricPrimitive::CreateSphere(graphics->GetD3DDeviceContext(), sphere.r * 2.0f);
		shape->Draw(Matrix::CreateTranslation(sphere.t), view, proj, color, nullptr, wireframe);
	}

	void Draw(const Capsule& capsule, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color, bool wireframe)
	{
		auto context = graphics->GetD3DDeviceContext();
		auto cylinder = GeometricPrimitive::CreateCylinder(context, capsule.hh * 2.0f, capsule.r * 2.0f);
		auto sphere = GeometricPrimitive::CreateSphere(context, capsule.r * 2.0f);

		Vector3 upper;
		Vector3 lower;
		CylinderBounds(capsule, upper, lower);

		sphere->Draw(Matrix::CreateTranslation(upper), view, proj, color, nullptr, wireframe);
		sphere->Draw(Matrix::CreateTranslation(lower), view, proj, color, nullptr, wireframe);
		cylinder->Draw(capsule.t.World(), view, proj, color, nullptr, wireframe);
	}
}
