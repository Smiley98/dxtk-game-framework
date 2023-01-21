#pragma once
#include "DXTK.h"
#include "MathUtilities.h"

class Renderer
{
public:
	//enum Type
	//{
	//	PLAYER,
	//	BUILDING
	//};

	virtual void Load(std::shared_ptr<DX::DeviceResources> graphics);
	virtual void Unload();

	// Do instanced eventually
	//virtual void Render(const std::vector<Matrix>& worlds, const Matrix& view, const Matrix& proj,
	//std::shared_ptr<DX::DeviceResources> graphics) = 0;

protected:
	std::unique_ptr<DirectX::CommonStates> mStates;

	// Might need to remove this eventually since things will most likely need multiple rendering passes
	std::shared_ptr<DirectX::BasicEffect> mShader;
};

