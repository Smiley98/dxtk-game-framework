#pragma once
#include <windef.h>
#include <array>
#include <memory>

#include "DeviceResources.h"
#include "Types.h"

class DirectX::GamePad;
class DirectX::Keyboard;
class DirectX::Mouse;
class DirectX::AudioEngine;

namespace scene
{
	class Scene
	{
	public:
		Scene() = delete;
		virtual ~Scene() = 0;

		// Constructors/destructors
		static void Create(DX::DeviceResources& graphics, DirectX::AudioEngine& audio);
		static void Destroy();

		// Handle window resize event
		static void Resize(int width, int height, DX::DeviceResources& graphics);

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

		static void Render(DX::DeviceResources& graphics);

		static Type Current();

	protected:
		Scene(DX::DeviceResources& graphics, DirectX::AudioEngine& audio);
		virtual void OnResize(int width, int height, DX::DeviceResources& graphics) = 0;

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

		virtual void OnRender(DX::DeviceResources& graphics) = 0;

	private:
		static std::array<Scene*, NONE> sScenes;
		static Type sType;
		bool m_paused = false;
	};
}
