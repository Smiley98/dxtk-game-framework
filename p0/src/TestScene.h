#pragma once
#include "Scene.h"

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

    void OnUpdate(float dt, float tt) final;
    void OnRender(std::shared_ptr<DX::DeviceResources> graphics) final;

private:
    void XM_CALLCONV DrawGrid(std::shared_ptr<DX::DeviceResources> graphics, DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color);
    
    // DirectXTK graphics objects
    std::unique_ptr<DirectX::CommonStates>                                  mStates;
    std::unique_ptr<DirectX::BasicEffect>                                   mBatchEffect;
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  mBatch;

    //std::unique_ptr<DirectX::SpriteBatch>                                   mSprites;
    //std::unique_ptr<DirectX::SpriteFont>                                    mFont;

    std::unique_ptr<DirectX::GeometricPrimitive>                            mShape;
    
    // D3D11 objects
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        mTexture1;
    //Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        mTexture2;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>                               mBatchInputLayout;

    Entity mParent, mChild1, mChild2;
};

// DirectXTK audio objects
//std::unique_ptr<DirectX::WaveBank>                mWaveBank;
//std::unique_ptr<DirectX::SoundEffect>             mSoundEffect;
//std::unique_ptr<DirectX::SoundEffectInstance>     mEffect1;
//std::unique_ptr<DirectX::SoundEffectInstance>     mEffect2;
// 
//std::unique_ptr<DirectX::EffectFactory>           mFxFactory;
//std::unique_ptr<DirectX::Model>                   mModel;