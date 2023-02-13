#pragma once
#include "Scene.h"
#include "Pathing.h"

class PathScene : public Scene
{
public:
	PathScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
	~PathScene() final;

	void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
	void OnBegin() final;
	void OnEnd() final;
	void OnPause() final;
	void OnResume() final;

	void OnUpdate(float dt, float tt) final;
	void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	const Tile::Map mMap
	{
		std::array<int, Tile::MAP_SIZE>{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
		std::array<int, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
		std::array<int, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
		std::array<int, Tile::MAP_SIZE>{ 3, 0, 0, 2, 2, 2, 2, 0, 0, 3 },
		std::array<int, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 2, 0, 0, 3 },
		std::array<int, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 2, 0, 0, 3 },
		std::array<int, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 2, 0, 0, 3 },
		std::array<int, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
		std::array<int, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
		std::array<int, Tile::MAP_SIZE>{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
	};
	Vector3 mMouseWorld;
	Pathing::Path mPath;

	Tile::Cell mStart;
	Tile::Cell mEnd;
	int mCurrent = 0;
	int mNext = 1;
	int mSteps = 0;

	std::vector<Tile::Node> mNodes;
	int mPathRenderState = 1;
};
