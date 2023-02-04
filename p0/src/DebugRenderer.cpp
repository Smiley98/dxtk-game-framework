#include "pch.h"
#include "DebugRenderer.h"

using namespace DirectX;

namespace Debug
{
	struct Line
	{
		Vector3 start;
		Vector3 end;
		float thickness = 0.0f;

		XMVECTOR color = Colors::White;
	};

	struct Sphere
	{
		Vector3 position;
		float radius = 0.0f;

		XMVECTOR color = Colors::White;
		bool wireframe = false;
	};

	struct Capsule
	{
		Vector3 position;
		Vector3 direction;
		float radius = 0.0f;
		float halfHeight = 0.0f;

		XMVECTOR color = Colors::White;
		bool wireframe = false;
	};

	struct FoV
	{
		Vector3 position;
		Vector3 direction;
		Vector3 target;
		float length = 0.0f;
		float degrees = 0.0f;

		XMVECTOR inColor = Colors::Green;
		XMVECTOR outColor = Colors::Red;
	};

	std::vector<Line> gLines;
	std::vector<Sphere> gSpheres;
	std::vector<Capsule> gCapsules;
	std::vector<FoV> gFoVs;

	void DrawSphere(const Sphere& sphere, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		auto shape = GeometricPrimitive::CreateSphere(graphics->GetD3DDeviceContext(), sphere.radius * 2.0f);
		shape->Draw(Matrix::CreateTranslation(sphere.position), view, proj, sphere.color, nullptr, sphere.wireframe);
	}

	void DrawCapsule(const Capsule& capsule, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
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

	void DrawLine(const Line& line, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
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

	void DrawFoV(const FoV& fov, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		Transform left, right;
		left.Translate(fov.position);
		right.Translate(fov.position);
		left.Orientate(fov.direction);
		right.Orientate(fov.direction);
		left.DeltaRotateZ(-fov.degrees * 0.5f);
		right.DeltaRotateZ(fov.degrees * 0.5f);

		XMVECTOR color = InFoV(fov.position, fov.direction, fov.target, fov.length, fov.degrees) ? Colors::Red : Colors::Green;
		DrawLine(fov.position, fov.position + left.Forward() * fov.length, 10.0f, view, proj, graphics, color);
		DrawLine(fov.position, fov.position + right.Forward() * fov.length, 10.0f, view, proj, graphics, color);
	}

	void DrawSphere(const Vector3& position, float radius,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color, bool wireframe, bool immediate)
	{
		Sphere sphere;
		sphere.color = color;
		sphere.wireframe = wireframe;
		sphere.position = position;
		sphere.radius = radius;
		if (immediate)
			DrawSphere(sphere, view, proj, graphics);
		else
			gSpheres.push_back(std::move(sphere));

		//auto shape = GeometricPrimitive::CreateSphere(graphics->GetD3DDeviceContext(), radius * 2.0f);
		//shape->Draw(Matrix::CreateTranslation(position), view, proj, color, nullptr, wireframe);
	}

	void DrawCapsule(const Vector3& position, const Vector3& direction, float radius, float halfHeight,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics,
		DirectX::XMVECTOR color, bool wireframe, bool immediate)
	{
		Capsule capsule;
		capsule.color = color;
		capsule.wireframe = wireframe;
		capsule.position = position;
		capsule.direction = direction;
		capsule.radius = radius;
		capsule.halfHeight = halfHeight;
		if (immediate)
			DrawCapsule(capsule, view, proj, graphics);
		else
			gCapsules.push_back(std::move(capsule));

		//auto context = graphics->GetD3DDeviceContext();
		//auto cylinder = GeometricPrimitive::CreateCylinder(context, halfHeight * 2.0f, radius * 2.0f);
		//auto sphere = GeometricPrimitive::CreateSphere(context, radius * 2.0f);
		//
		//Vector3 upper;
		//Vector3 lower;
		//CylinderBounds(position, direction, halfHeight, upper, lower);
		//
		//Transform transform;
		//transform.Translate(position);
		//transform.Orientate(direction);
		//
		//sphere->Draw(Matrix::CreateTranslation(upper), view, proj, color, nullptr, wireframe);
		//sphere->Draw(Matrix::CreateTranslation(lower), view, proj, color, nullptr, wireframe);
		//cylinder->Draw(transform.Local(), view, proj, color, nullptr, wireframe);
	}

	void DrawLine(const Vector3& a, const Vector3& b, float thickness, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, DirectX::XMVECTOR color, bool immediate)
	{
		Line line;
		line.color = color;
		line.start = a;
		line.end = b;
		line.thickness = thickness;
		if (immediate)
			DrawLine(line, view, proj, graphics);
		else
			gLines.push_back(std::move(line));

		//Vector3 line = b - a;
		//float length = line.Length();
		//line.Normalize();
		//
		//Transform transform;
		//transform.Translate(a + line * length * 0.5f);
		//transform.Orientate(line);
		//
		//auto box = GeometricPrimitive::CreateBox(graphics->GetD3DDeviceContext(), { thickness, length, 1.0f });
		//box->Draw(transform.Local(), view, proj, color);
	}

	void DrawFoV(const Vector3& position, const Vector3& direction, const Vector3& target, float length, float degrees,
		const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics, bool immediate)
	{
		FoV fov;
		fov.position = position;
		fov.direction = direction;
		fov.target = target;
		fov.length = length;
		fov.degrees = degrees;
		if (immediate)
			DrawFoV(fov, view, proj, graphics);
		else
			gFoVs.push_back(std::move(fov));

		//Transform left, right;
		//left.Translate( position);
		//right.Translate(position);
		//left.Orientate(direction);
		//right.Orientate(direction);
		//left.DeltaRotateZ(-fov * 0.5f);
		//right.DeltaRotateZ(fov * 0.5f);
		//
		//XMVECTOR color = InFoV(position, direction, target, length, fov) ? Colors::Red : Colors::Green;
		//DrawLine(position, position + left.Forward() * length, 10.0f, view, proj, graphics, color);
		//DrawLine(position, position + right.Forward() * length, 10.0f, view, proj, graphics, color);
	}

	void DrawDeferred(const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		for (const Sphere& sphere : gSpheres)
			DrawSphere(sphere, view, proj, graphics);

		for (const Capsule& capsule : gCapsules)
			DrawCapsule(capsule, view, proj, graphics);

		for (const Line& line : gLines)
			DrawLine(line, view, proj, graphics);

		for (const FoV& fov : gFoVs)
			DrawFoV(fov, view, proj, graphics);

		gSpheres.clear();
		gCapsules.clear();
		gLines.clear();
		gFoVs.clear();
	}

	void DrawPrimitive(Geometry geometry, const Matrix& world, const Matrix& view, const Matrix& proj,
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
}

//Matrix::CreateWorld(position, direction, Vector3::UnitZ);
// This does right-up-forward instead of forward-right-up (XYZ vs ZXY)...