#include "pch.h"
#include "TestScene.h"

#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Audio.h"

namespace scene
{
	TestScene::TestScene(DX::DeviceResources& graphics, DirectX::AudioEngine& audio) : Scene(graphics, audio)
	{
	}

	TestScene::~TestScene()
	{
	}

	void TestScene::OnResize(int width, int height, DX::DeviceResources& graphics)
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

	void TestScene::OnRender(DX::DeviceResources& graphics)
	{
	}
}
