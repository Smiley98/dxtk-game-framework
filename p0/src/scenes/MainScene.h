#pragma once
#include "Scene.h"

namespace scene
{
	class MainScene :
		public Scene
	{
	public:
		MainScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
		~MainScene() final;

	protected:
		virtual void OnResize(std::shared_ptr<DX::DeviceResources> graphics) override;

		virtual void OnBegin() override;
		virtual void OnEnd() override;

		virtual void OnPause() override;
		virtual void OnResume() override;

		virtual void OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse) override;
		virtual void OnRender(std::shared_ptr<DX::DeviceResources> graphics) override;

	private:
		std::unique_ptr<DirectX::CommonStates> m_states;
		//std::unique_ptr<DirectX::GeometricPrimitive> m_shape;	// Better to use Model so we have automatic bounds and identical desired workflow
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture; // No need for textures yet
		std::unique_ptr<DirectX::Model>	m_vbo;
		std::shared_ptr<DirectX::BasicEffect> m_shader;

		DirectX::SimpleMath::Matrix m_world;
		DirectX::SimpleMath::Matrix	m_view;
		DirectX::SimpleMath::Matrix	m_projection;

	};
}

