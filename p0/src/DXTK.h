#pragma once

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

#include "SimpleMath.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"