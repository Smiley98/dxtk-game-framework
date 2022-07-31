#include "pch.h"
#include "DebugRenderer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Debug
{
	void Primitive(Type geometry, const Matrix& world, const Matrix& view, const Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color)
	{
		auto context = graphics->GetD3DDeviceContext();
		switch (geometry)
		{
		case Debug::BOX:
			GeometricPrimitive::CreateCube(context)->Draw(world, view, proj, color);
			break;
		case Debug::SPHERE:
			GeometricPrimitive::CreateSphere(context)->Draw(world, view, proj, color);
			break;
		case Debug::CYLINDER:
			GeometricPrimitive::CreateCylinder(context)->Draw(world, view, proj, color);
			break;
		case Debug::CONE:
			GeometricPrimitive::CreateCone(context)->Draw(world, view, proj, color);
			break;
		case Debug::TORUS:
			GeometricPrimitive::CreateTorus(context)->Draw(world, view, proj, color);
			break;
		case Debug::TEAPOT:
			GeometricPrimitive::CreateTeapot(context)->Draw(world, view, proj, color);
			break;
		default:
			break;
		}
	}

	void Draw(const Collision::SphereCollider& collider, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color)
	{
		auto context = graphics->GetD3DDeviceContext();
	}

	void Draw(const Collision::CapsuleCollider& collider, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color)
	{
		auto context = graphics->GetD3DDeviceContext();
	}

}
