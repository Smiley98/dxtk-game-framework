#include "pch.h"
#include "TestScene.h"

using namespace DirectX;
namespace scene
{
	TestScene::TestScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio) : Scene(graphics, audio)
	{
		m_waveBank = std::make_unique<WaveBank>(audio.get(), L"assets/sounds/adpcmdroid.xwb");
		m_soundEffect = std::make_unique<SoundEffect>(audio.get(), L"assets/music/MusicMono_adpcm.wav");
		m_effect1 = m_soundEffect->CreateInstance();
		m_effect2 = m_waveBank->CreateInstance(10);
	}

	TestScene::~TestScene()
	{
	}

	void TestScene::OnResize(int width, int height, std::shared_ptr<DX::DeviceResources> graphics)
	{
	}

	void TestScene::OnBegin()
	{
		m_effect1->Play(true);
		m_effect2->Play();
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

	void TestScene::OnRender(std::shared_ptr<DX::DeviceResources> graphics)
	{
	}
}
