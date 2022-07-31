#pragma once
#include "Scene.h"

namespace Scene
{
    class TestScene :
        public Scene
    {
    public:
        TestScene(std::shared_ptr<DX::DeviceResources> graphics, std::shared_ptr<DirectX::AudioEngine> audio);
        ~TestScene() final;

    protected:
        void OnResize(std::shared_ptr<DX::DeviceResources> graphics) final;

        void OnBegin() final;
        void OnEnd() final;

        void OnPause() final;
        void OnResume() final;

        void OnUpdate(const DX::StepTimer& timer, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse) final;
        void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

    private:
        // DirectXTK audio objects
        std::unique_ptr<DirectX::WaveBank>              m_waveBank;
        std::unique_ptr<DirectX::SoundEffect>           m_soundEffect;
        std::unique_ptr<DirectX::SoundEffectInstance>   m_effect1;
        std::unique_ptr<DirectX::SoundEffectInstance>   m_effect2;

        // DirectXTK graphics objects
        std::unique_ptr<DirectX::CommonStates>                                  m_states;
        std::unique_ptr<DirectX::BasicEffect>                                   m_batchEffect;
        std::unique_ptr<DirectX::EffectFactory>                                 m_fxFactory;
        std::unique_ptr<DirectX::GeometricPrimitive>                            m_shape;
        std::unique_ptr<DirectX::Model>                                         m_model;
        std::unique_ptr<DirectX::Model>                                         m_van;
        std::shared_ptr<DirectX::BasicEffect>                                   m_vanEffect;
        std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch;
        std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites;
        std::unique_ptr<DirectX::SpriteFont>                                    m_font;

        // D3D11 objects
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture1;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture2;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_textureVan;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>                               m_batchInputLayout;

        // Graphics objects
        DirectX::SimpleMath::Matrix                                             m_world;
        DirectX::SimpleMath::Matrix                                             m_view;
        DirectX::SimpleMath::Matrix                                             m_projection;

        void XM_CALLCONV DrawGrid(std::shared_ptr<DX::DeviceResources> graphics, DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color);
    };
}
