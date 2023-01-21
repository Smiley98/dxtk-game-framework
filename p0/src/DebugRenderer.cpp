#include "pch.h"
#include "DebugRenderer.h"

using namespace DirectX;

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
		Vector3 line = b - a;
		float length = line.Length();
		line.Normalize();

		Transform transform;
		transform.Translate(a + line * length * 0.5f);
		transform.Orientate(line);
		
		auto box = GeometricPrimitive::CreateBox(graphics->GetD3DDeviceContext(), { thickness, length, 1.0f });
		box->Draw(transform.Local(), view, proj, color);
	}

	void InRange(const Vector3& position, const Vector3& direction, const Vector3& target, float length, float fov,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		Transform left, right;
		left.Translate( position);
		right.Translate(position);
		left.Orientate(direction);
		right.Orientate(direction);
		left.DeltaRotateZ(-fov * 0.5f);
		right.DeltaRotateZ(fov * 0.5f);

		XMVECTOR color = ::InRange(position, direction, target, length, fov) ? Colors::Red : Colors::Green;
		Line(position, position + left.Forward() * length, 10.0f, view, proj, graphics, color);
		Line(position, position + right.Forward() * length, 10.0f, view, proj, graphics, color);
	}

	void Sphere(const Vector3& position, float radius,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color, bool wireframe)
	{
		auto shape = GeometricPrimitive::CreateSphere(graphics->GetD3DDeviceContext(), radius * 2.0f);
		shape->Draw(Matrix::CreateTranslation(position), view, proj, color, nullptr, wireframe);
	}

	void Capsule(const Vector3& position, const Vector3& direction, float radius, float halfHeight,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color, bool wireframe)
	{
		auto context = graphics->GetD3DDeviceContext();
		auto cylinder = GeometricPrimitive::CreateCylinder(context, halfHeight * 2.0f, radius * 2.0f);
		auto sphere = GeometricPrimitive::CreateSphere(context, radius * 2.0f);

		Vector3 upper;
		Vector3 lower;
		CylinderBounds(position, direction, halfHeight, upper, lower);

		Transform transform;
		transform.Translate(position);
		transform.Orientate(direction);
		//Matrix::CreateWorld(position, direction, Vector3::UnitZ);
		// This does right-up-forward instead of forward-right-up (XYZ vs ZXY)...

		sphere->Draw(Matrix::CreateTranslation(upper), view, proj, color, nullptr, wireframe);
		sphere->Draw(Matrix::CreateTranslation(lower), view, proj, color, nullptr, wireframe);
		cylinder->Draw(transform.Local(), view, proj, color, nullptr, wireframe);
	}
}
