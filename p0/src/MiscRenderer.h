#pragma once
#include "Renderer.h"

class MiscRenderer :
	public Renderer
{
public:
	void Load(std::shared_ptr<DX::DeviceResources> graphics) final;
	void Unload() final;

	void Cone(const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics) const;

	void Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2,
		const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics) const;

private:
	std::shared_ptr<DirectX::Model> mCone;

	//std::shared_ptr<DirectX::BasicEffect> mTriangleShader;
	//std::unique_ptr<DirectX::PrimitiveBatch<GeometricPrimitive::VertexType>> mTriangleBatch;
	//Microsoft::WRL::ComPtr<ID3D11InputLayout> mTriangleLayout;
};