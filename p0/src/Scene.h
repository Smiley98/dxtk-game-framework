#pragma once
#include "DXTK.h"
#include "SceneType.h"
#include <array>
#include <memory>

namespace DX
{
	class DeviceResources;
	class StepTimer;
}

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
		static void Resize(std::shared_ptr<DX::DeviceResources> graphics);

		// Sets internal scene to passed in scene and calls its begin handler (starts the game)
		static void Run(Type type);

		// Ends the current scene and begins [type] scene 
		static void Change(Type type);

		static void Pause();
		static void Resume();

		static void Update(
			const DX::StepTimer& timer,
			const DirectX::GamePad& gamePad,
			const DirectX::Keyboard& keyboard,
			const DirectX::Mouse& mouse
		);

		static void Render(std::shared_ptr<DX::DeviceResources> graphics);

		static Type Current();

	protected:
		Scene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
		virtual void OnResize(std::shared_ptr<DX::DeviceResources> graphics) = 0;

		virtual void OnBegin() = 0;
		virtual void OnEnd() = 0;

		virtual void OnPause() = 0;
		virtual void OnResume() = 0;

		virtual void OnUpdate(
			const DX::StepTimer& timer,
			const DirectX::GamePad& gamePad,
			const DirectX::Keyboard& keyboard,
			const DirectX::Mouse& mouse
		) = 0;

		virtual void OnRender(std::shared_ptr<DX::DeviceResources> graphics) = 0;

	private:
		static std::array<Scene*, NONE> sScenes;
		static Type sType;
		bool mPaused = false;
	};
}
