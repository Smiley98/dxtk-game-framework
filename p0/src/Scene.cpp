#include "pch.h"
#include "Scene.h"
#include "TestScene.h"
#include "MainScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
namespace scene
{
	std::array<Scene*, NONE> Scene::sScenes;
	Type Scene::sType = NONE;

	std::shared_ptr<DirectX::BasicEffect> Scene::sVanShader;
	std::shared_ptr<DirectX::BasicEffect> Scene::sBuildingShader;
	std::shared_ptr<DirectX::Model> Scene::sVan;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Scene::sVanTexture;

	Scene::Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Create(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio)
	{
		auto context = graphics->GetD3DDeviceContext();
		auto device = graphics->GetD3DDevice();

		DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"assets/textures/van.dds", nullptr, sVanTexture.ReleaseAndGetAddressOf()));
		sVanShader = std::make_shared<BasicEffect>(device);
		sVanShader->EnableDefaultLighting();
		sVanShader->SetLightDirection(0, Vector3::UnitZ * -1.0f + Vector3::UnitX * -1.0f);
		sVanShader->SetTextureEnabled(true);
		sVanShader->SetTexture(sVanTexture.Get());
		sVan = Model::CreateFromVBO(device, L"assets/meshes/van.vbo", sVanShader);

		sBuildingShader = std::make_shared<BasicEffect>(device);
		sBuildingShader->EnableDefaultLighting();
		sBuildingShader->SetLightDirection(0, Vector3::UnitZ * -1.0f + Vector3::UnitX * -1.0f);
		sBuildingShader->SetTextureEnabled(false);

		//sScenes[TEST] = new TestScene(graphics, audio);
		//sScenes[SPLASH] = new SplashScene(graphics, audio);
		//sScenes[LOADOUT] = new LoadoutScene(graphics, audio);
		//sScenes[MAP] = new MapScene(graphics, audio);
		sScenes[MAIN] = new MainScene(graphics, audio);
	}

	void Scene::Destroy()
	{
		sBuildingShader.reset();
		sVanShader.reset();
		sVanTexture.Reset();
		sVan.reset();

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

	Type Scene::Current()
	{
		return sType;
	}
}
