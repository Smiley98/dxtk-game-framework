#pragma once
#include "Scene.h"

namespace scene
{
    class TestScene :
        public Scene
    {
    public:
        TestScene(DX::DeviceResources& graphics, DirectX::AudioEngine& audio);
        ~TestScene() final;

    protected:
        void OnResize(int width, int height, DX::DeviceResources& graphics) final;

        void OnBegin() final;
        void OnEnd() final;

        void OnPause() final;
        void OnResume() final;

        void OnUpdate(const double dt, const DirectX::GamePad& gamePad, const DirectX::Keyboard& keyboard, const DirectX::Mouse& mouse) final;
        void OnRender(DX::DeviceResources& graphics) final;

    private:

    };
}
