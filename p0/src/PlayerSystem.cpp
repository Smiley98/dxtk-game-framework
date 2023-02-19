#include "pch.h"
#include "PlayerSystem.h"
#include "Dynamics.h"
#include "Components.h"

#define GAMEPAD false
#define KEYBOARD true

namespace Players
{
	void Update(Components& components, float dt)
	{
		using namespace DirectX;
		using namespace SimpleMath;

		for (size_t i = 0; i < components.players.Count(); i++)
		{
			Entity entity = components.players.GetEntity(i);
			EntityTransform& transform = *components.transforms.Get(entity);
			Rigidbody& body = *components.rigidbodies.Get(entity);
			Player& player = *components.players.Get(entity);

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
			Keyboard::State keys = Keyboard::Get().GetState();
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

			switch (translation)
			{
			case FORWARD:
				player.acc = VAN_MOVE_ACC;
				break;

			case BACKWARD:
				player.acc = -VAN_MOVE_ACC;
				break;

			case NONE:
				player.acc = 0.0f;
				player.vel *= 0.95f;
				break;
			}

			// Double acceleration if braking
			player.acc *= player.vel > 0.0f && player.acc < 0.0f || player.vel < 0.0f && player.acc > 0.0f ? 2.0f : 1.0f;
			player.vel += player.acc * dt;
			player.vel = std::clamp(player.vel, -VAN_MOVE_VEL, VAN_MOVE_VEL);
			transform.DeltaTranslate(transform.Forward() * Dynamics::Integrate(player.vel, player.acc, dt));

			// Write velocity to rigidbody so other physics objects interact correctly with Players
			// (Could write acceleration as well, but it won't be accurate due to 1D motion)
			body.velocity = transform.Forward() * player.vel;
		}
	}
}