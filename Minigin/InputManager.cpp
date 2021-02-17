#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


bool dae::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);
	UpdateControllerCommandMap();

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

dae::InputState dae::InputManager::IsPressed(unsigned id, ControllerButton button) const
{
	auto buttonIt = std::find_if(m_Controls.begin(), m_Controls.end(), [id, button](const std::pair<ControllerKey, ControllerCommand>& pair)
	{
			return pair.first.first == id && pair.first.second == button;
	});

	if (buttonIt == m_Controls.end()) return InputState::up;
	const ButtonInfo& buttonInfo = buttonIt->second.first;

	if (buttonInfo.pressed && !buttonInfo.lastValue) return InputState::pressed;
	if (buttonInfo.pressed && buttonInfo.lastValue) return InputState::down;
	if (!buttonInfo.pressed && buttonInfo.lastValue) return InputState::released;
	return InputState::up;
}

void dae::InputManager::Bind(ControllerButton button, const std::shared_ptr<Command>& command)
{
	//For loop in case there are multiple controllers
	for (auto& pair : m_Controls)
	{
		if (pair.first.second == button) pair.second.second = command;
	}
}

void dae::InputManager::UpdateControllerCommandMap()
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
	}
}

