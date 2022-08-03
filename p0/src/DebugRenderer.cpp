#include "pch.h"
#include "DebugRenderer.h"
#include "Collision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Debug
{
	void Primitive(Geometry geometry, const Matrix& world, const Matrix& view, const Matrix& proj,
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

	void Draw(const SphereCollider& collider, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color)
	{
		auto sphere = GeometricPrimitive::CreateSphere(graphics->GetD3DDeviceContext(), collider.radius * 2.0f);
		sphere->Draw(Matrix::CreateTranslation(collider.translation), view, proj, color);
	}

	void Draw(const CapsuleCollider& collider, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj,
		std::shared_ptr<DX::DeviceResources> graphics, XMVECTOR color)
	{
		Vector3 forward = collider.transform.Front();
		Matrix top = Matrix::CreateTranslation(collider.transform.Translation() + forward *  collider.halfHeight);
		Matrix bot = Matrix::CreateTranslation(collider.transform.Translation() + forward * -collider.halfHeight);

		auto context = graphics->GetD3DDeviceContext();
		auto cylinder = GeometricPrimitive::CreateCylinder(context, collider.halfHeight * 2.0f, collider.radius * 2.0f);
		auto sphere = GeometricPrimitive::CreateSphere(context, collider.radius * 2.0f);
		sphere->Draw(top, view, proj, color);
		sphere->Draw(bot, view, proj, color);
		cylinder->Draw(collider.transform.World(), view, proj, color);
	}
}
