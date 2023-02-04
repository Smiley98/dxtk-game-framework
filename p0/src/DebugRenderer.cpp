#include "pch.h"
#include "DebugRenderer.h"
#include "Geometry.h"

using namespace DirectX;

namespace Debug
{
	// And yet some people say multi-inheritance shouldn't exist :p xD
	struct GeometryRender
	{
		XMVECTOR color = Colors::White;
		bool wireframe = false;
	};

	struct SphereRender : public GeometryRender, public Sphere {};
	struct CapsuleRender : public GeometryRender, public Capsule {};

	struct LineRender : public Line
	{
		float thickness = 0.0f;
		XMVECTOR color = Colors::White;
	};

	struct FoVRender : public FoV
	{
		float thickness = 0.0f;
		XMVECTOR inColor = Colors::Green;
		XMVECTOR outColor = Colors::Red;
	};

	std::vector<SphereRender> gSpheres;
	std::vector<CapsuleRender> gCapsules;
	std::vector<LineRender> gLines;
	std::vector<FoVRender> gFoVs;

	void DrawSphere(const Vector3& position, float radius, XMVECTOR color, bool wireframe)
	{
		SphereRender sphere;
		sphere.position = position;
		sphere.radius = radius;

		sphere.color = color;
		sphere.wireframe = wireframe;
		gSpheres.push_back(std::move(sphere));
	}

	void DrawCapsule(const Vector3& position, const Vector3& direction, float radius, float halfHeight, XMVECTOR color, bool wireframe)
	{
		CapsuleRender capsule;
		capsule.position = position;
		capsule.direction = direction;
		capsule.radius = radius;
		capsule.halfHeight = halfHeight;

		capsule.color = color;
		capsule.wireframe = wireframe;
		gCapsules.push_back(std::move(capsule));
	}

	void DrawLine(const Vector3& a, const Vector3& b, float thickness, XMVECTOR color)
	{
		LineRender line;
		line.start = a;
		line.end = b;

		line.color = color;
		line.thickness = thickness;
		gLines.push_back(std::move(line));
	}

	void DrawFoV(const Vector3& position, const Vector3& direction, const Vector3& target, float length, float degrees,
		float thickness, XMVECTOR inColor, XMVECTOR outColor)
	{
		FoVRender fov;
		fov.position = position;
		fov.direction = direction;
		fov.target = target;
		fov.length = length;
		fov.degrees = degrees;

		fov.thickness = thickness;
		fov.inColor = inColor;
		fov.outColor = outColor;
		gFoVs.push_back(std::move(fov));
	}

	void DrawSphere(const SphereRender& sphere, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		auto shape = GeometricPrimitive::CreateSphere(graphics->GetD3DDeviceContext(), sphere.radius * 2.0f);
		shape->Draw(Matrix::CreateTranslation(sphere.position), view, proj, sphere.color, nullptr, sphere.wireframe);
	}

	void DrawCapsule(const CapsuleRender& capsule, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		auto context = graphics->GetD3DDeviceContext();
		auto cylinder = GeometricPrimitive::CreateCylinder(context, capsule.halfHeight * 2.0f, capsule.radius * 2.0f);
		auto sphere = GeometricPrimitive::CreateSphere(context, capsule.radius * 2.0f);

		Vector3 upper;
		Vector3 lower;
		CylinderBounds(capsule.position, capsule.direction, capsule.halfHeight, upper, lower);

		Transform transform;
		transform.Translate(capsule.position);
		transform.Orientate(capsule.direction);

		sphere->Draw(Matrix::CreateTranslation(upper), view, proj, capsule.color, nullptr, capsule.wireframe);
		sphere->Draw(Matrix::CreateTranslation(lower), view, proj, capsule.color, nullptr, capsule.wireframe);
		cylinder->Draw(transform.Local(), view, proj, capsule.color, nullptr, capsule.wireframe);
	}

	void DrawLine(const LineRender& line, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		Vector3 direction = line.end - line.start;
		float length = direction.Length();
		direction.Normalize();

		Transform transform;
		transform.Translate(line.start + direction * length * 0.5f);
		transform.Orientate(direction);

		auto box = GeometricPrimitive::CreateBox(graphics->GetD3DDeviceContext(), { line.thickness, length, 1.0f });
		box->Draw(transform.Local(), view, proj, line.color);
	}

	void DrawFoV(const FoVRender& fov, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		Transform left, right;
		left. Translate(fov.position);
		right.Translate(fov.position);
		left. Orientate(fov.direction);
		right.Orientate(fov.direction);
		left.DeltaRotateZ(-fov.degrees * 0.5f);
		right.DeltaRotateZ(fov.degrees * 0.5f);

		LineRender leftLine, rightLine;
		rightLine.end = fov.position + right.Forward() * fov.length;
		leftLine.end = fov.position + left.Forward() * fov.length;
		leftLine.start = rightLine.start = fov.position;
		leftLine.thickness = rightLine.thickness = fov.thickness;
		leftLine.color = rightLine.color =
			InFoV(fov.position, fov.direction, fov.target, fov.length, fov.degrees) ? fov.inColor : fov.outColor;

		DrawLine(leftLine,  view, proj, graphics);
		DrawLine(rightLine, view, proj, graphics);
	}

	void DrawDeferred(const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		for (const SphereRender& sphere : gSpheres)
			DrawSphere(sphere, view, proj, graphics);

		for (const CapsuleRender& capsule : gCapsules)
			DrawCapsule(capsule, view, proj, graphics);

		for (const LineRender& line : gLines)
			DrawLine(line, view, proj, graphics);

		for (const FoVRender& fov : gFoVs)
			DrawFoV(fov, view, proj, graphics);

		gSpheres.clear();
		gCapsules.clear();
		gLines.clear();
		gFoVs.clear();
	}

	void Draw(Primitive primitive, const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color, bool wireframe)
	{
		auto context = graphics->GetD3DDeviceContext();
		switch (primitive)
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
}
