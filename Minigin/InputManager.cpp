#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


dae::InputManager::InputManager()
{
	const int amountOfControllers{ 1 };
	for (int controllerId = 0; controllerId < amountOfControllers; ++controllerId)
	{
		for (int i = 0; i < int(ControllerButton::Last); ++i)
		{
			ControllerButton cb = ControllerButton(i);
			ControllerKey ck = std::make_pair(controllerId, cb);
			ButtonInfo bi{ false, false, InputState::up };
			ControllerCommand cc = std::make_pair(bi, nullptr);
			m_Controls.emplace(ck, cc);
		}
	}
}

bool dae::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);
	ProcessControllerInput();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	return true;
}

void dae::InputManager::Bind(ControllerButton button, const std::shared_ptr<Command>& command, InputState inputState)
{
	//For loop in case there are multiple controllers
	for (auto& pair : m_Controls)
	{
		if (pair.first.second == button)
		{
			pair.second.second = command;
			pair.second.first.stateRequired = inputState;
		}
	}
}

bool dae::InputManager::ButtonInfo::isActive() const
{
	switch (stateRequired)
	{
		case InputState::pressed:
			return pressed && !lastValue;
		case InputState::released:
			return !pressed && lastValue;
		case InputState::down:
			return pressed;
		case InputState::up: 
			return !pressed;
		default: 
			return false;
	}
}

void dae::InputManager::ProcessControllerInput()
{
	for (auto& pair : m_Controls)
	{
		ControllerCommand& cc = pair.second;
		ButtonInfo& bi = cc.first;
		bi.lastValue = bi.pressed;

		const ControllerKey& ck = pair.first;
		switch (ck.second)
		{
		case ControllerButton::ButtonA:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
			break;
		case ControllerButton::ButtonB:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
			break;
		case ControllerButton::ButtonX:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
			break;
		case ControllerButton::ButtonY:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
			break;
		case ControllerButton::ButtonStart:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_START;
			break;
		case ControllerButton::ButtonBack:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
			break;
		case ControllerButton::ShoulderLeft:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			break;
		case ControllerButton::ShoulderRight:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
			break;
		case ControllerButton::DpadLeft:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			break;
		case ControllerButton::DpadRight:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
			break;
		case ControllerButton::DpadUp:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
			break;
		case ControllerButton::DpadDown:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			break;
		case ControllerButton::ThumbLeft:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
			break;
		case ControllerButton::ThumbRight:
			cc.first.pressed = m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
			break;
		default:
			cc.first.pressed = false;
		}

		const ButtonInfo& buttonInfo = pair.second.first;
		if (buttonInfo.isActive() && pair.second.second != nullptr) pair.second.second->Execute();
	}
}

