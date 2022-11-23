#pragma once
#define WIN32_LEAN_AND_MEAN

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

namespace DX
{
	class DeviceResources;
	class StepTimer;
}

#include "PrimitiveBatch.h"
#include "VertexTypes.h"
