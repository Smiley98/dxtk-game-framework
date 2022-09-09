#include "pch.h"
#include "Scene.h"
#include "TestScene.h"
#include "CollisionScene.h"
#include "EntityScene.h"
#include "MainScene.h"

#include "Buildings.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::array<Scene*, Scene::NONE> Scene::sScenes;
Scene::Type Scene::sType = NONE;

PlayerRenderer Scene::sPlayerRenderer;
MiscRenderer Scene::sMiscRenderer;

Scene::Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
{
}

Scene::~Scene()
{
}

void Scene::Create(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
{
	sPlayerRenderer.Load(graphics);
	sMiscRenderer.Load(graphics);
	Building::Load(graphics);

	//sScenes[SPLASH] = new SplashScene(graphics, audio);
	//sScenes[LOADOUT] = new LoadoutScene(graphics, audio);
	//sScenes[MAP] = new MapScene(graphics, audio);
	//sScenes[MAIN] = new MainScene(graphics, audio);
	//sScenes[TEST] = new TestScene(graphics, audio);
	//sScenes[COLLISION] = new CollisionScene(graphics, audio);
	//sScenes[PHYSICS] = new PhysicsScene(graphics, audio);
	sScenes[ENTITY] = new EntityScene(graphics, audio);
}

void Scene::Destroy()
{
	Building::Unload();
	sPlayerRenderer.Unload();
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

void Scene::Update(float dt, float tt, DX::Input& input)
{
	sScenes[sType]->OnUpdate(dt, tt, input);

	auto& timers = sScenes[sType]->mTimers;
	for (auto& [key, val] : timers)
	{
		val.Update(dt);
		if (val.IsExpired() /* && !val.IsRepeat() <-- unnecessary because repeating timers never expire*/)
			timers.erase(key);
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
	mTimers.erase(name);
}
