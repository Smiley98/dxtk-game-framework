#pragma once
#include "DXTK.h"
#include "Input.h"
#include "Timer.h"

#include "PlayerRenderer.h"
#include "BuildingRenderer.h"
#include "MiscRenderer.h"
#include "DebugRenderer.h"

#include <array>
#include <memory>
#include <string>
#include <queue>

#include "Components.h"

class Scene
{
public:
	enum Type {
		// TODO -- Make an actual video game xD xD xD
		//SPLASH,
		//LOADOUT,
		//MAP,
		//MAIN,

		TEST,
		COLLISION,
		STEERING,
		SPLINE,
		ENTITY,
		NONE
	};

	Scene() = delete;
	virtual ~Scene() = 0;

	// Constructors/destructors
	static void Create(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components);
	static void Destroy();

	// Handle window resize event
	static void Resize(std::shared_ptr<DX::DeviceResources> graphics);

	// Sets internal scene to passed in scene and calls its begin handler (starts the game)
	static void Run(Type type);

	// Ends the current scene and begins [type] scene 
	static void Change(Type type);

	static void Pause();
	static void Resume();

	static void Update(float dt, float tt, const DX::Input& input);
	static void Render(std::shared_ptr<DX::DeviceResources> graphics);

	static Type Current();

protected:
	Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components);
	virtual void OnResize(std::shared_ptr<DX::DeviceResources> graphics) = 0;

	virtual void OnBegin() = 0;
	virtual void OnEnd() = 0;

	virtual void OnPause() = 0;
	virtual void OnResume() = 0;

	virtual void OnUpdate(float dt, float tt, const DX::Input& input) = 0;
	virtual void OnRender(std::shared_ptr<DX::DeviceResources> graphics) = 0;

	void AddTimer(const std::string& name, float duration, TimerCallback callback, bool repeat);
	void RemoveTimer(const std::string& name);

	void AddProcess(const std::string& name, float frequency, float duration, ProcessCallback callback, bool repeat);
	void RemoveProcess(const std::string& name);

	// Shared members
	static PlayerRenderer sPlayerRenderer;
	static BuildingRenderer sBuildingRenderer;
	static MiscRenderer sMiscRenderer;

	// Entities shared between scenes
	static Entity sPlayer;

	// Common members
	Matrix mView;
	Matrix mProj;
	const float mWorldWidth = 1600;
	const float mWorldHeight = 900;

	Components& mComponents;

private:
	static std::array<Scene*, NONE> sScenes;
	static Type sType;
	bool mPaused = false;

	std::unordered_map<std::string, Timer> mTimers;
	std::unordered_map<std::string, Process> mProcesses;
	std::queue<std::string> mTimerRemoveQueue;
	std::queue<std::string> mProcessRemoveQueue;
};
