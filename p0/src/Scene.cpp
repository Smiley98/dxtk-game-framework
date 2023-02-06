#include "pch.h"
#include "Scene.h"
#include "TestScene.h"
#include "CollisionScene.h"
#include "SteeringScene.h"
#include "SplineScene.h"
#include "EntityScene.h"
#include "PathScene.h"
#include "MainScene.h"

#include "PlayerFactory.h"
#include "DebugRenderer.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::array<Scene*, Scene::NONE> Scene::sScenes;
Scene::Type Scene::sType = NONE;

PlayerRenderer Scene::sPlayerRenderer;
BuildingRenderer Scene::sBuildingRenderer;
MiscRenderer Scene::sMiscRenderer;

std::unordered_map<Entity, uint32_t> gLookup;	// Global component hashes (see ComponentMap.h)
Components Scene::sComponents;					// All components (shared between scenes)
Entity Scene::sPlayer = INVALID_ENTITY;			// Player shared between scenes

Scene::Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
{
}

Scene::~Scene()
{
}

void Scene::Create(
	std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Type type)
{
	EntityTransform::sComponents = &sComponents;

	sPlayerRenderer.Load(graphics);
	sBuildingRenderer.Load(graphics);
	sMiscRenderer.Load(graphics);

	sPlayer = CreatePlayer(sComponents, sPlayerRenderer);
	sComponents.transforms.GetComponent(sPlayer)->Translate(800.0f, 450.0f, 0.0f);

	//sScenes[SPLASH] = new SplashScene(graphics, audio);
	//sScenes[LOADOUT] = new LoadoutScene(graphics, audio);
	//sScenes[MAP] = new MapScene(graphics, audio);
	//sScenes[MAIN] = new MainScene(graphics, audio);

	// Note, be careful when debugging with multiple scenes at a time.
	// For example, instantiating the same map 2+ times breaks collisions.
	
	sType = type;
	//sScenes[COLLISION] = new CollisionScene(graphics, audio);
	//sScenes[STEERING] = new SteeringScene(graphics, audio);
	//sScenes[SPLINE] = new SplineScene(graphics, audio);
	sScenes[PATH] = new PathScene(graphics, audio);
	//sScenes[ENTITY] = new EntityScene(graphics, audio);
	//sScenes[TEST] = new TestScene(graphics, audio);
}

void Scene::Destroy()
{
	sPlayerRenderer.Unload();
	sBuildingRenderer.Unload();
	sMiscRenderer.Unload();

	// Temporarily disabled so I can run a single scene without crashing
	//sType = NONE;
	//for (Scene* scene : sScenes)
	//{
	//	delete scene;
	//	scene = nullptr;
	//}
}

void Scene::Resize(std::shared_ptr<DX::DeviceResources> graphics)
{
	// Temporarily disabled so I can run a single scene without crashing
	//for (Scene* scene : sScenes)
	//	scene->OnResize(graphics);

	sScenes[sType]->OnResize(graphics);
}

void Scene::Run()
{
	sScenes[sType]->OnBegin();
}

void Scene::Change(Type type)
{
	sScenes[sType]->mTimers.clear();
	sScenes[sType]->OnEnd();
	sType = type;
	sScenes[sType]->OnBegin();
}

void Scene::Pause()
{
	Scene*& scene = sScenes[sType];
	if (!scene->mPaused)
		scene->Pause();
	scene->mPaused = true;
}

void Scene::Resume()
{
	Scene*& scene = sScenes[sType];
	if (scene->mPaused)
		scene->Resume();
	scene->mPaused = false;
}

void Scene::Update(float dt, float tt)
{
	Scene& scene = *sScenes[sType];
	scene.OnUpdate(dt, tt);

	for (auto& [key, val] : scene.mTimers)
		val.Tick(dt);

	for (auto& [key, val] : scene.mProcesses)
		val.Tick(dt);

	while (!scene.mTimerRemoveQueue.empty())
	{
		scene.mTimers.erase(scene.mTimerRemoveQueue.front());
		scene.mTimerRemoveQueue.pop();
	}

	while (!scene.mProcessRemoveQueue.empty())
	{
		scene.mProcesses.erase(scene.mProcessRemoveQueue.front());
		scene.mProcessRemoveQueue.pop();
	}

	for (auto it = scene.mTimers.begin(); it != scene.mTimers.end();)
	{
		if (it->second.IsExpired())
			it = scene.mTimers.erase(it);
		else
			it++;
	}

	for (auto it = scene.mProcesses.begin(); it != scene.mProcesses.end();)
	{
		if (it->second.IsExpired())
			it = scene.mProcesses.erase(it);
		else
			it++;
	}
}

void Scene::Render(std::shared_ptr<DX::DeviceResources> graphics)
{
	Scene& scene = *sScenes[sType];
	scene.OnRender(graphics);
	Debug::DrawDeferred(scene.mView, scene.mProj, graphics);
}

Scene::Type Scene::Current()
{
	return sType;
}

void Scene::AddTimer(const std::string& name, float duration, TimerCallback callback, bool repeat)
{
	Timer timer(duration, callback, repeat);
	mTimers.insert({ name, std::move(timer) });
}

void Scene::RemoveTimer(const std::string& name)
{
	mTimerRemoveQueue.push(name);
}

void Scene::AddProcess(const std::string& name, float frequency, float duration, ProcessCallback callback, bool repeat)
{
	Process process(frequency, duration, callback, repeat);
	mProcesses.insert({ name, std::move(process) });
}

void Scene::RemoveProcess(const std::string& name)
{
	mProcessRemoveQueue.push(name);
}
