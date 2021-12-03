#include "FreeCameraController.h"
#include "Engine.h"

namespace pbls
{
	void pbls::FreeCameraController::Update()
	{
		glm::vec3 rotate{ 0 };
		if (owner->scene->engine->Get<InputSystem>()->GetButtonState((int)InputSystem::eMouseButton::Right) == InputSystem::eKeyState::Held)
		{
			glm::vec2 axis = owner->scene->engine->Get<InputSystem>()->GetMouseRelative() * sensitivity;
			rotate.x -= glm::radians(axis.y);
			rotate.y -= glm::radians(axis.x);
		}

		owner->transform.rotation += rotate;

		glm::vec3 direction{ 0 };
		if (owner->scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == pbls::InputSystem::eKeyState::Held) direction.x = -1;
		if (owner->scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == pbls::InputSystem::eKeyState::Held) direction.x = 1;
		if (owner->scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == pbls::InputSystem::eKeyState::Held) direction.z = -1;
		if (owner->scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_S) == pbls::InputSystem::eKeyState::Held) direction.z = 1;
		if (owner->scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_E) == pbls::InputSystem::eKeyState::Held) direction.y = 1;
		if (owner->scene->engine->Get<pbls::InputSystem>()->GetKeyState(SDL_SCANCODE_Q) == pbls::InputSystem::eKeyState::Held) direction.y = -1;

		owner->transform.position += direction * speed * owner->scene->engine->time.deltaTime;
	}

	bool pbls::FreeCameraController::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool pbls::FreeCameraController::Read(const rapidjson::Value& value)
	{
		return false;
	}

}
