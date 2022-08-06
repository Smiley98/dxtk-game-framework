#include "pch.h"
#include "Scene.h"
#include "TestScene.h"
#include "CollisionScene.h"
#include "MainScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::array<Scene*, Scene::NONE> Scene::sScenes;
Scene::Type Scene::sType = NONE;

Vector3 Scene::sLightDirection { 0.0f, 0.0f, -1.0f };
Vector3 Scene::sAmbient	{ Vector3::One * 0.5f };
Vector3 Scene::sDiffuse	{ Vector3::One * 0.75f };
Vector3 Scene::sSpecular{ Vector3::One * 0.1f };

std::shared_ptr<DirectX::Model> Scene::sVan;
std::shared_ptr<DirectX::BasicEffect> Scene::sVanShader;
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Scene::sVanTexture;

Scene::Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) :
	mStates(std::make_unique<CommonStates>(graphics->GetD3DDevice()))
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
	sVanShader->SetAmbientLightColor(sAmbient);
	sVanShader->SetDiffuseColor(sDiffuse);
	sVanShader->SetSpecularColor(sSpecular);
	sVanShader->SetSpecularPower(256.0f);
	sVanShader->SetLightDirection(0, sLightDirection);
	sVanShader->SetTextureEnabled(true);
	sVanShader->SetTexture(sVanTexture.Get());
	sVan = Model::CreateFromVBO(device, L"assets/meshes/van.vbo", sVanShader);

	//sScenes[SPLASH] = new SplashScene(graphics, audio);
	//sScenes[LOADOUT] = new LoadoutScene(graphics, audio);
	//sScenes[MAP] = new MapScene(graphics, audio);
	//sScenes[MAIN] = new MainScene(graphics, audio);
	//sScenes[TEST] = new TestScene(graphics, audio);
	sScenes[COLLISION] = new CollisionScene(graphics, audio);
	//sScenes[PHYSICS] = new PhysicsScene(graphics, audio);
}

void Scene::Destroy()
{
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

Scene::Type Scene::Current()
{
	return sType;
}