#include "pch.h"
#include "Scene.h"
#include "TestScene.h"

namespace scene
{
	std::array<Scene*, NONE> Scene::sScenes;
	Type Scene::sType = NONE;

	Scene::Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Create(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	{
		sScenes[TEST] = new TestScene(graphics, audio);
		//sScenes[SPLASH] = new SplashScene;
		//sScenes[LOADOUT] = new LoadoutScene;
		//sScenes[MAP] = new MapScene;
		//sScenes[MAIN] = new MainScene;
	}

	void Scene::Destroy()
	{
		sType = NONE;
		for (Scene* scene : sScenes)
		{
			delete scene;
			scene = nullptr;
		}
	}

	void Scene::Resize(int width, int height, std::shared_ptr<DX::DeviceResources> graphics)
	{
		for (Scene* scene : sScenes)
			scene->OnResize(width, height, graphics);
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
		if (!scene->m_paused)
			scene->Pause();
		scene->m_paused = true;
	}

	void Scene::Resume()
	{
		Scene*& scene = sScenes[sType];
		if (scene->m_paused)
			scene->Resume();
		scene->m_paused = false;
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

	Type Scene::Current()
	{
		return sType;
	}
}
