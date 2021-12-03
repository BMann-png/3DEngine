#include "InputSystem.h"

namespace pbls
{
	void InputSystem::Startup()
	{
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(&numKeys);

		keyboardState.resize(numKeys);

		std::copy(keyboardStateSDL + 0, keyboardStateSDL + numKeys - 1, keyboardState.begin());

		prevKeyboardState = keyboardState;

		// set initial mouse position
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		mousePosition = glm::vec2{ x , y };
		prevMousePosition = mousePosition;
	}

	void InputSystem::Shutdown()
	{
	}

	void InputSystem::Update(float dt)
	{
		prevKeyboardState = keyboardState;

		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(nullptr);
		std::copy(keyboardStateSDL + 0, keyboardStateSDL + numKeys - 1, keyboardState.begin());

		prevMouseButtonState = mouseButtonState;
		prevMousePosition = mousePosition;
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		mousePosition = { x, y };
		mouseButtonState[0] = buttons & SDL_BUTTON_LMASK;
		mouseButtonState[1] = buttons & SDL_BUTTON_MMASK;
		mouseButtonState[2] = buttons & SDL_BUTTON_RMASK;
		mouseRelative = mousePosition - prevMousePosition;
	}

	InputSystem::eKeyState InputSystem::GetKeyState(int id)
	{
		eKeyState state = eKeyState::Idle;
		bool keyDown = IsKeyDown(id);
		bool prevKeyDown = IsPreviousKeyDown(id);
		if (keyDown) 
		{ 
			state = (prevKeyDown) ? state = eKeyState::Held : state = eKeyState::Pressed;
		}
		else 
		{ 
			state = (prevKeyDown) ? state = eKeyState::Released : state = eKeyState::Idle;
		}
		return state;
	}

	InputSystem::eKeyState InputSystem::GetButtonState(int id)
	{
		eKeyState state = eKeyState::Idle;
		bool keyDown = IsButtonDown(id);
		bool prevKeyDown = IsPreviousButtonDown(id);
		if (keyDown)
		{
			state = (prevKeyDown) ? state = eKeyState::Held : state = eKeyState::Pressed;
		}
		else
		{
			state = (prevKeyDown) ? state = eKeyState::Released : state = eKeyState::Idle;
		}
		return state;
	}
}