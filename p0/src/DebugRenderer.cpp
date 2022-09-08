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

	void Line(const Vector3& a, const Vector3& b, float thickness,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color)
	{
		//Vector3 line = b - a;
		//float length = line.Length() * 0.5f;
		//Quaternion orientation = Quaternion::FromToRotation(a, b);
		//Matrix world = XMMatrixTransformation(
		//	g_XMZero, Quaternion::Identity, g_XMOne,
		//	g_XMZero, orientation, line * 0.5f);
		//
		//auto box = GeometricPrimitive::CreateBox(graphics->GetD3DDeviceContext(), { thickness, length, 1.0f });
		//box->Draw(world, view, proj, color);
	}

	void Draw(const Collision::SphereCollider& collider,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color, bool wireframe)
	{
		auto shape = GeometricPrimitive::CreateSphere(graphics->GetD3DDeviceContext(), collider.Radius() * 2.0f);
		shape->Draw(Matrix::CreateTranslation(collider.position), view, proj, color, nullptr, wireframe);
	}

	void Draw(const Collision::CapsuleCollider& collider,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color, bool wireframe)
	{
		auto context = graphics->GetD3DDeviceContext();
		auto cylinder = GeometricPrimitive::CreateCylinder(context, collider.HalfHeight() * 2.0f, collider.Radius() * 2.0f);
		auto sphere = GeometricPrimitive::CreateSphere(context, collider.Radius() * 2.0f);

		Vector3 upper;
		Vector3 lower;
		Collision::CylinderBounds(collider, collider.HalfHeight(), upper, lower);

		sphere->Draw(Matrix::CreateTranslation(upper), view, proj, color, nullptr, wireframe);
		sphere->Draw(Matrix::CreateTranslation(lower), view, proj, color, nullptr, wireframe);
		cylinder->Draw(collider.World(), view, proj, color, nullptr, wireframe);
	}
}
