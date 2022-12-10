#include "pch.h"
#include "Constants.h"
#include "PlayerSystem.h"
#include "Components.h"
#define GAMEPAD false
#define KEYBOARD true

namespace Players
{
	void Update(Components& components, const DX::Input& input, float dt)
	{
		using namespace DirectX;
		using namespace SimpleMath;

		for (size_t i = 0; i < components.players.Count(); i++)
		{
			Entity entity = components.players.GetEntity(i);
			Kinematic& body = *components.bodies.GetComponent(entity);
			Player& player = *components.players.GetComponent(entity);
			Transform& transform = *components.transforms.GetComponent(entity);

#if GAMEPAD
			GamePad::State state = input.gamePad.GetState(0);

			if (state.IsLeftThumbStickLeft())
				transform.DeltaRotateZ(av);
			if (state.IsLeftThumbStickRight())
				transform.DeltaRotateZ(-av);

			if (state.IsAPressed())
				transform.DeltaTranslate(transform.Forward() * av);
			if (state.IsXPressed())
				transform.DeltaTranslate(transform.Forward() * -av);
#endif
			
#if KEYBOARD
			Keyboard::State state = input.keyboard.GetState();
			if (state.W)
				body.hAcc = Constants::VAN_H_ACC_MAX;
			else if (state.S)
				body.hAcc = -Constants::VAN_H_ACC_MAX;
			else
				body.hAcc = 0.0f;

			if (state.A)
				transform.DeltaRotateZ(player.steering * dt);
			if (state.D)
				transform.DeltaRotateZ(-player.steering * dt);
#endif
			body.hVel = std::clamp(body.hVel, -Constants::VAN_H_VEL_MAX, Constants::VAN_H_VEL_MAX);
		}
	}
}
