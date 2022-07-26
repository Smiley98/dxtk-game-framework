#pragma once

// D3D11 and STL
//#include <d3d11.h>
#include <array>
#include <memory>

namespace DX
{
	class DeviceResources;
}

// Input
class DirectX::GamePad;
class DirectX::Keyboard;
class DirectX::Mouse;

// Audio
class DirectX::AudioEngine;
class DirectX::WaveBank;
class DirectX::SoundEffect;
class DirectX::SoundEffectInstance;

// Graphics
class DirectX::CommonStates;
class DirectX::BasicEffect;
class DirectX::EffectFactory;
class DirectX::GeometricPrimitive;
class DirectX::Model;
class DirectX::SpriteBatch;
class DirectX::SpriteFont;

#include "Types.h"
namespace scene
{
	class Scene
	{
	public:
		Scene() = delete;
		virtual ~Scene() = 0;

		// Constructors/destructors
		static void Create(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
		static void Destroy();

		// Handle window resize event
		static void Resize(int width, int height, std::shared_ptr<DX::DeviceResources> graphics);

		// Sets internal scene to passed in scene and calls its begin handler (starts the game)
		static void Run(Type type);

		// Ends the current scene and begins [type] scene 
		static void Change(Type type);

		static void Pause();
		static void Resume();

		static void Update(
			const double dt,
			const DirectX::GamePad& gamePad,
			const DirectX::Keyboard& keyboard,
			const DirectX::Mouse& mouse
		);

		static void Render(std::shared_ptr<DX::DeviceResources> graphics);

		static Type Current();

	protected:
		Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
		virtual void OnResize(int width, int height, std::shared_ptr<DX::DeviceResources> graphics) = 0;

		virtual void OnBegin() = 0;
		virtual void OnEnd() = 0;

		virtual void OnPause() = 0;
		virtual void OnResume() = 0;

		virtual void OnUpdate(
			const double dt,
			const DirectX::GamePad& gamePad,
			const DirectX::Keyboard& keyboard,
			const DirectX::Mouse& mouse
		) = 0;

		virtual void OnRender(std::shared_ptr<DX::DeviceResources> graphics) = 0;

	private:
		static std::array<Scene*, NONE> sScenes;
		static Type sType;
		bool m_paused = false;
	};
}
