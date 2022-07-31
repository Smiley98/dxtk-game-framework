#pragma once
#include "DeviceResources.h"
#include "Collision.h"

class DebugRenderer
{
public:
	static void Create(std::shared_ptr<DX::DeviceResources> graphics);
	static void Destroy();

	static void Draw(const collision::SphereCollider& collider);
	static void Draw(const collision::CapsuleCollider& collider);

private:
	static std::shared_ptr<DX::DeviceResources> sGraphics;
};

