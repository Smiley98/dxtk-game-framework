#include "pch.h"
#include "MiscRenderer.h"
#include "Scene.h"

using namespace DirectX;

void MiscRenderer::Load(std::shared_ptr<DX::DeviceResources> graphics)
{
	Renderer::Load(graphics);
	auto device = graphics->GetD3DDevice();

	mShader = std::make_shared<BasicEffect>(device);
	mShader->EnableDefaultLighting();
	mCone = Model::CreateFromVBO(device, L"assets/meshes/cone.vbo", mShader);
}

void MiscRenderer::Unload()
{
	mCone.reset();
	mFont.reset();
	mSprites.reset();
	Renderer::Unload();
}

void MiscRenderer::Cone(const Matrix& world, const Matrix& view, const Matrix& proj,
	std::shared_ptr<DX::DeviceResources> graphics) const
{
	mCone->Draw(graphics->GetD3DDeviceContext(), *mStates, world, view, proj);
}

void MiscRenderer::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2,
	const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics) const
{
	Vector3 normal;
	{
		Vector3 edge1 = p1 - p0;
		Vector3 edge2 = p2 - p0;
		Vector3 ortho = edge1.Cross(edge2);
		ortho.Normalize(normal);
	}

	GeometricPrimitive::VertexType v0;
	GeometricPrimitive::VertexType v1;
	GeometricPrimitive::VertexType v2;

	v0.position = p0;
	v1.position = p1;
	v2.position = p2;

	v0.textureCoordinate = { 1.0f, 0.0f };
	v1.textureCoordinate = { 1.0f, 1.0f };
	v2.textureCoordinate = { 0.0f, 1.0f };

	v0.normal = v1.normal = v2.normal = normal;

	GeometricPrimitive::VertexCollection vertices{ v0, v1, v2 };
	GeometricPrimitive::IndexCollection indices{ 0, 1, 2 };

	auto context = graphics->GetD3DDeviceContext();
	auto triangle = GeometricPrimitive::CreateCustom(context, vertices, indices);

	triangle->Draw(Matrix::Identity, view, proj);
}
