#include "pch.h"
#include "TestScene.h"

namespace scene
{
	TestScene::TestScene(std::weak_ptr<DX::DeviceResources> graphics, std::weak_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
	{
		//m_waveBank = std::make_unique<WaveBank>(&audio, L"../../assets/sounds/adpcmdroid.xwb");
	}

	TestScene::~TestScene()
	{
	}

	void TestScene::OnResize(int width, int height, std::weak_ptr<DX::DeviceResources> graphics)
	{
	}

	void TestScene::OnBegin()
	{
	}

	void TestScene::OnEnd()
	{
	}

	void TestScene::OnPause()
	{
	}

	void TestScene::OnResume()
	{
	}

	void TestScene::OnUpdate(const double dt, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse)
	{
	}

	void TestScene::OnRender(std::weak_ptr<DX::DeviceResources> graphics)
	{
	}
}
