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
		float x = (b - a).Length() * 0.5f;
		float y = thickness;

		// instead of doing this sorcery (atan2 between a and origin + a and b), we should figure out Orientate()!
		// (find midpoint of segment (b - a) * 0.5f, find direction vector, then move to position and orientation.
		//float a = atan2(a.y, a.x);
		//float b = atan2(b.y - a.y, b.x - a.x);

		auto context = graphics->GetD3DDeviceContext();
		auto box = GeometricPrimitive::CreateBox(context, { x, y, 1.0f });
		//box->Draw()
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
