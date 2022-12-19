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

			enum MovementState {
				FORWARD,
				BACKWARD,
				NONE
			} translation = NONE, rotation = NONE;

#if GAMEPAD
			GamePad::State pad = GamePad::Get().GetState(i);

			if (pad.IsAPressed())
				translation = FORWARD;
			else if (pad.IsXPressed())
				translation = BACKWARD;
			else
				translation = NONE;

			if (pad.IsLeftThumbStickLeft())
				rotation = FORWARD;
			else if (pad.IsLeftThumbStickRight())
				rotation = BACKWARD;
			else
				rotation = NONE;
#endif
			
#if KEYBOARD
			Keyboard::State keys = input.keyboard.GetState();
			if (keys.W)
				translation = FORWARD;
			else if (keys.S)
				translation = BACKWARD;
			else
				translation = NONE;

			if (keys.A)
				rotation = FORWARD;
			if (keys.D)
				rotation = BACKWARD;
#endif

			switch (translation)
			{
			case FORWARD:
				body.hAcc = VAN_MOVE_ACC;
				break;

			case BACKWARD:
				body.hAcc = -VAN_MOVE_ACC;
				break;

			case NONE:
				body.hAcc = 0.0f;
				body.hVel *= 0.95f;
				break;
			}

			switch (rotation)
			{
			case FORWARD:
				transform.DeltaRotateZ(player.steering * dt);
				break;

			case BACKWARD:
				transform.DeltaRotateZ(-player.steering * dt);
				break;

			case NONE:
				break;
			}

			// Increase deceleration (via frication) if breaking (opposite to current velocity)
			if (body.hVel > 0.0f && body.hAcc < 0.0f || body.hVel < 0.0f && body.hAcc > 0.0f)
			{
				body.hVel *= 0.9f;
			}
			body.hVel = std::clamp(body.hVel, -VAN_MOVE_VEL, VAN_MOVE_VEL);
		}
	}
}
