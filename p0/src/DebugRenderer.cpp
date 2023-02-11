#include "pch.h"
#include "DebugRenderer.h"
#include "Geometry.h"
#include "Scene.h"

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
	struct BoxRender : public GeometryRender, public Box {};

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

	struct TextRender
	{
		Vector3 position;
		std::wstring characters;
		XMVECTOR color = Colors::White;
		bool centred;
	};

	std::vector<SphereRender> gSpheres;
	std::vector<CapsuleRender> gCapsules;
	std::vector<BoxRender> gBoxes;
	std::vector<LineRender> gLines;
	std::vector<FoVRender> gFoVs;
	std::vector<TextRender> gTexts;

	std::unique_ptr<SpriteBatch> gSprites;
	std::unique_ptr<SpriteFont> gFont;

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

	void DrawBox(const Vector3& position, const Vector3& extents, const Vector3& direction, DirectX::XMVECTOR color, bool wireframe)
	{
		BoxRender box;
		box.position = position;
		box.direction = direction;
		box.extents = extents;

		box.color = color;
		box.wireframe = wireframe;
		gBoxes.push_back(std::move(box));
	}

	void DrawLine(const Vector3& start, const Vector3& end, float thickness, XMVECTOR color)
	{
		LineRender line;
		line.start = start;
		line.end = end;

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

	void DrawText(const Vector3& position, const std::wstring& characters, DirectX::XMVECTOR color, bool centred)
	{
		TextRender text;
		text.position = position;
		text.characters = characters;
		text.color = color;
		text.centred = centred;
		gTexts.push_back(std::move(text));
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

	void DrawBox(const BoxRender& box, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics)
	{
		auto cube = GeometricPrimitive::CreateCube(graphics->GetD3DDeviceContext());

		Transform transform;
		transform.Translate(box.position);
		transform.Orientate(box.direction);
		transform.Scale(box.extents * 2.0f);

		cube->Draw(transform.Local(), view, proj, box.color, nullptr, box.wireframe);
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

		for (const BoxRender& box : gBoxes)
			DrawBox(box, view, proj, graphics);

		for (const LineRender& line : gLines)
			DrawLine(line, view, proj, graphics);

		for (const FoVRender& fov : gFoVs)
			DrawFoV(fov, view, proj, graphics);

		gSprites->Begin();
		for (const TextRender& text : gTexts)
		{
			Vector3 screenPoint = Scene::WorldToScreen(text.position);
			const wchar_t* characters = text.characters.c_str();
			if (text.centred)
			{
				Vector2 size = gFont->MeasureString(characters);
				screenPoint.x -= size.x * 0.5f;
				screenPoint.y -= size.y * 0.5f;
			}
			gFont->DrawString(gSprites.get(), characters, XMFLOAT2{ screenPoint.x, screenPoint.y }, text.color);
		}
		gSprites->End();

		gSpheres.clear();
		gCapsules.clear();
		gBoxes.clear();
		gLines.clear();
		gFoVs.clear();
		gTexts.clear();
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

	void Load(std::shared_ptr<DX::DeviceResources> graphics)
	{
		gSprites = std::make_unique<SpriteBatch>(graphics->GetD3DDeviceContext());
		gFont = std::make_unique<SpriteFont>(graphics->GetD3DDevice(), L"assets/fonts/SegoeUI_18.spritefont");
	}

	void Unload()
	{
		gFont.reset();
		gSprites.reset();
	}
}
