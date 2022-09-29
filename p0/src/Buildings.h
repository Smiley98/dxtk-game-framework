#pragma once
#include "Collision.h"

class Building
{
public:

	// Type is used to index into constants like durabilities and models.
	// We could even have common collider bounds, but that's too extreme.
	enum Type : size_t
	{
		TD,
		APARTMENT,
		BMO,
		CONDO,
		DUPLEX,
		OFFICE,
		PENTA,
		PINK,
		COUNT
	} type;

	Collision::StaticCapsule collider;
	Vector3 position;
	float hitpoints;
	Color color = { 0.75f, 0.75f, 0.75f };

	static void Load(std::shared_ptr<DX::DeviceResources> graphics);
	static void Unload();

	// Test renderer
	static void Draw(const Building& building, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics);

	// Instanced renderer
	//void Draw(const std::vector<Building>& buildings, const Matrix& view, const Matrix& proj, std::shared_ptr<DX::DeviceResources> graphics);

	static const DirectX::Model& Model(Type type);
	static Vector3 Bounds(Type type);
	static float Durability(Type type);

private:
	static std::unique_ptr<DirectX::CommonStates> sStates;
	static std::shared_ptr<DirectX::BasicEffect> sShader;
	static std::array<std::unique_ptr<DirectX::Model>, COUNT> sModels;
	static std::array<float, COUNT> sDurabilities;
};
