#include "pch.h"
//#include "Constants.h"
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

			constexpr float VAN_MOVE_VEL = 500.0f;
			constexpr float VAN_MOVE_ACC = 500.0f;

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
				body.hAcc = VAN_MOVE_ACC;
			else if (state.S)
				body.hAcc = -VAN_MOVE_ACC;
			else
			{
				body.hAcc = 0.0f;
				body.hVel *= 0.95f;
			}

			// Increase acceleration (via frication) if breaking (opposite to current velocity)
			if (body.hVel > 0.0f && body.hAcc < 0.0f || body.hVel < 0.0f && body.hAcc > 0.0f)
			{
				body.hVel *= 0.9f;
			}

			if (state.A)
				transform.DeltaRotateZ(player.steering * dt);
			if (state.D)
				transform.DeltaRotateZ(-player.steering * dt);
#endif
			body.hVel = std::clamp(body.hVel, -VAN_MOVE_VEL, VAN_MOVE_VEL);
		}
	}
}
