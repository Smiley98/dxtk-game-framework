#include "pch.h"
#include "Scene.h"
#include "TestScene.h"
#include "CollisionScene.h"
#include "EntityScene.h"
#include "MainScene.h"
#include "Entity.h"

std::unordered_map<Entity, uint32_t> gLookup;

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::array<Scene*, Scene::NONE> Scene::sScenes;
Scene::Type Scene::sType = NONE;

PlayerRenderer Scene::sPlayerRenderer;
BuildingRenderer Scene::sBuildingRenderer;
MiscRenderer Scene::sMiscRenderer;

Scene::Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components) :
	mComponents(components)
{
}

Scene::~Scene()
{
}

void Scene::Create(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio, Components& components)
{
	sPlayerRenderer.Load(graphics);
	sBuildingRenderer.Load(graphics);
	sMiscRenderer.Load(graphics);

	//sScenes[SPLASH] = new SplashScene(graphics, audio, components);
	//sScenes[LOADOUT] = new LoadoutScene(graphics, audio, components);
	//sScenes[MAP] = new MapScene(graphics, audio, components);
	//sScenes[MAIN] = new MainScene(graphics, audio, components);
	sScenes[TEST] = new TestScene(graphics, audio, components);
	sScenes[COLLISION] = new CollisionScene(graphics, audio, components);
	//sScenes[PHYSICS] = new PhysicsScene(graphics, audio, components);
	sScenes[ENTITY] = new EntityScene(graphics, audio, components);
}

void Scene::Destroy()
{
	sPlayerRenderer.Unload();
	sBuildingRenderer.Unload();
	sMiscRenderer.Unload();

	sType = NONE;
	for (Scene* scene : sScenes)
	{
		delete scene;
		scene = nullptr;
	}
}

void Scene::Resize(std::shared_ptr<DX::DeviceResources> graphics)
{
	for (Scene* scene : sScenes)
		scene->OnResize(graphics);
}

void Scene::Run(Type type)
{
	sType = type;
	sScenes[type]->OnBegin();
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

void Scene::Update(float dt, float tt, const DX::Input& input)
{
	Scene& scene = *sScenes[sType];
	scene.OnUpdate(dt, tt, input);

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
	sScenes[sType]->OnRender(graphics);
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
