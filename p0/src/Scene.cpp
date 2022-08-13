#include "pch.h"
#include "Scene.h"
#include "TestScene.h"
#include "CollisionScene.h"
#include "EntityScene.h"
#include "MainScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::array<Scene*, Scene::NONE> Scene::sScenes;
Scene::Type Scene::sType = NONE;

BuildingRenderer Scene::sBuildingRenderer;
PlayerRenderer Scene::sPlayerRenderer;

Scene::Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
{
}

Scene::~Scene()
{
}

void Scene::Create(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
{
	sPlayerRenderer.Load(graphics);
	sBuildingRenderer.Load(graphics);

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
	sBuildingRenderer.Unload();
	sPlayerRenderer.Unload();

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

void Scene::Update(
	const DX::StepTimer& timer,
	const DirectX::GamePad& gamePad,
	const DirectX::Keyboard& keyboard,
	const DirectX::Mouse& mouse
)
{
	sScenes[sType]->OnUpdate(timer, gamePad, keyboard, mouse);
}

void Scene::Render(std::shared_ptr<DX::DeviceResources> graphics)
{
	sScenes[sType]->OnRender(graphics);
}

Scene::Type Scene::Current()
{
	return sType;
}