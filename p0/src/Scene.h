#pragma once
#include "DXTK.h"
#include "Input.h"
#include "Timer.h"

#include "PlayerRenderer.h"
#include "BuildingRenderer.h"
#include "MiscRenderer.h"

#include "Component.h"
#include "Tags.h"

#include <array>
#include <memory>
#include <string>

using TimerCallback = std::function<void()>;

class Scene
{
public:
	enum Type {
		//SPLASH,
		//LOADOUT,
		//MAP,
		//MAIN,
		TEST,
		COLLISION,
		//PHYSICS,
		ENTITY,
		NONE
	};

	Scene() = delete;
	virtual ~Scene() = 0;

	// Constructors/destructors
	static void Create(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
	static void Destroy();

	// Handle window resize event
	static void Resize(std::shared_ptr<DX::DeviceResources> graphics);

	// Sets internal scene to passed in scene and calls its begin handler (starts the game)
	static void Run(Type type);

	// Ends the current scene and begins [type] scene 
	static void Change(Type type);

	static void Pause();
	static void Resume();

	static void Update(float dt, float tt, DX::Input& input);
	static void Render(std::shared_ptr<DX::DeviceResources> graphics);

	static Type Current();

protected:
	Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
	virtual void OnResize(std::shared_ptr<DX::DeviceResources> graphics) = 0;

	virtual void OnBegin() = 0;
	virtual void OnEnd() = 0;

	virtual void OnPause() = 0;
	virtual void OnResume() = 0;

	virtual void OnUpdate(float dt, float tt, DX::Input& input) = 0;
	virtual void OnRender(std::shared_ptr<DX::DeviceResources> graphics) = 0;

	void AddTimer(const std::string& name, float duration, TimerCallback callback, bool repeat);
	void RemoveTimer(const std::string& name);

	// Shared members
	static PlayerRenderer sPlayerRenderer;
	static BuildingRenderer sBuildingRenderer;
	static MiscRenderer sMiscRenderer;

	// Common members
	Matrix mView;
	Matrix mProj;

	// Components (should always be allocated by collections within scene)
	ComponentCollection<Transform3> mTransforms;
	ComponentCollection<Collision2::SphereCollider> mSpheres;
	ComponentCollection<Collision2::CapsuleCollider> mCapsules;
	ComponentCollection<Tags::Tag> mTags;

private:
	static std::array<Scene*, NONE> sScenes;
	static Type sType;
	bool mPaused = false;

	std::unordered_map<std::string, Timer> mTimers;
};
