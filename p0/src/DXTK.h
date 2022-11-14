#pragma once
#define WIN32_LEAN_AND_MEAN
#include <wrl/client.h>
#include <d3d11_1.h>
#include <dxgi1_6.h>

namespace DirectX
{
	// Input
	class GamePad;
	class Keyboard;
	class Mouse;

	// Audio
	class AudioEngine;
	class WaveBank;
	class SoundEffect;
	class SoundEffectInstance;

	// Graphics
	class CommonStates;
	class BasicEffect;
	class EffectFactory;
	class GeometricPrimitive;
	class Model;
	class SpriteBatch;
	class SpriteFont;
}

#include "Collision.h"
#include "Collision2.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"

#include "DeviceResources.h"
#include "StepTimer.h"