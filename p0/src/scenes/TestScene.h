#pragma once
#include "Scene.h"

namespace scene
{
    class TestScene :
        public Scene
    {
    public:
        TestScene(std::weak_ptr<DX::DeviceResources> graphics, std::weak_ptr<DirectX::AudioEngine> audio);
        ~TestScene() final;

    protected:
        void OnResize(int width, int height, std::weak_ptr<DX::DeviceResources> graphics) final;

        void OnBegin() final;
        void OnEnd() final;

        void OnPause() final;
        void OnResume() final;

        void OnUpdate(const double dt, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse) final;
        void OnRender(std::weak_ptr<DX::DeviceResources> graphics) final;

    private:
        //std::unique_ptr<DirectX::WaveBank> m_waveBank;
    };
}
