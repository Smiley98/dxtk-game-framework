#pragma once
#include "Scene.h"
#include "TileMap.h"

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
		std::array<size_t, Tile::MAP_SIZE>{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
		std::array<size_t, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		std::array<size_t, Tile::MAP_SIZE>{ 3, 0, 1, 1, 1, 1, 1, 1, 1, 0 },
		std::array<size_t, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		std::array<size_t, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		std::array<size_t, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		std::array<size_t, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		std::array<size_t, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		std::array<size_t, Tile::MAP_SIZE>{ 3, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
		std::array<size_t, Tile::MAP_SIZE>{ 3, 2, 1, 3, 2, 1, 3, 2, 1, 0 },
	};
};
