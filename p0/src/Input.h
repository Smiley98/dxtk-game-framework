#pragma once
#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"

namespace DX
{
	struct Input
	{
		DirectX::GamePad	gamePad;
		DirectX::Keyboard	keyboard;
		DirectX::Mouse		mouse;
	};
}
