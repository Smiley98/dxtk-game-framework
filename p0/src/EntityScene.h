#pragma once
#include "Scene.h"
#include "Collision.h"
#include "Map.h"
#include "Curves.h"
#include <unordered_map>

class GameObject
{
public:
	GameObject* const Parent()
	{
		return mParent;
	}

	void SetParent(GameObject* parent)
	{
		if (mParent != nullptr)
			mParent->RemoveChild(mName);

		parent->AddChild(this);
		mParent = parent;
	}

	GameObject* Child(const std::string& name)
	{
		return mChildren.at(name);
	}

	void AddChild(GameObject* child)
	{
		child->mParent = this;
		mChildren.insert({ child->Name(), child });
	}

	void RemoveChild(const std::string& name)
	{
		mChildren.at(name)->mParent = nullptr;
		mChildren.erase(name);
	}

	std::string Name()
	{
		return mName;
	}

	void SetName(const std::string& name)
	{
		if (mParent != nullptr)
			mParent->RemoveChild(mName);
		mName = name;
		if (mParent != nullptr)
			mParent->AddChild(this);
	}

private:
	RigidTransform mTransform;
	std::string mName;

	GameObject* mParent = nullptr;
	std::unordered_map<std::string, GameObject*> mChildren;
};

struct Player
{
	Collision::DynamicCapsule id;
	Objects::Player type = Objects::VAN;
	RigidTransform* transform;

	void Load(const PlayerRenderer& renderer, Collision::Colliders& colliders)
	{
		Vector3 bounds = renderer.Bounds(type);
		colliders.Add(id, bounds.y - bounds.x, bounds.x, Tags::PLAYER, this);
		transform = colliders.Get(id);
	}
};

class EntityScene :
	public Scene
{
public:
	EntityScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
	~EntityScene() final;

	void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;
	void OnBegin() final;
	void OnEnd() final;
	void OnPause() final;
	void OnResume() final;
	void OnUpdate(float dt, float tt, DX::Input& input) final;
	void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
	Collision::Colliders mColliders;
	MintyAftershave mMap;

	Player mVan;
	Building mBuilding;

	Transform mHeadlights;
	const float fov = 60.0f;
	const float range = 400.0f;

	float mWorldWidth = 1600;
	float mWorldHeight = 900;

	std::vector<Vector3> mSpline = {
		Vector3{ 500.0f, -300.0f, 0.0f },
		Vector3{ -500.0f, -300.0f, 100.0f },
		Vector3{ -500.0f, 300.0f, 100.0f },
		Vector3{ 500.0f, 300.0f, 0.0f }
	};
	SpeedTable mSpeedTable;

	float t = 0.0f, d = 0.0f;
	size_t interval = 0, sample = 0;

	GameObject mParent, mChildA, mChildB;
};
