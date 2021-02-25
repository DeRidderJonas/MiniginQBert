#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


dae::InputManager::InputManager()
{
}

bool dae::InputManager::ProcessInput()
{
	for (int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		XINPUT_STATE inputState{};
		ZeroMemory(&inputState, sizeof(XINPUT_STATE));
		XInputGetState(i, &inputState);
		ProcessControllerInput(inputState, i);
	}

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

void dae::InputManager::Bind(unsigned controllerId, ControllerButton button, const std::shared_ptr<Command>& command, InputState inputState)
{
	ControllerKey controllerKey = std::make_pair(controllerId, button);
	auto duplicateIt = m_Controls.find(controllerKey);
	if (duplicateIt != m_Controls.end()) throw std::runtime_error("Button was already mapped for controller");

	ButtonInfo buttonInfo{ false, false, inputState };
	ControllerCommand controllerCommand = std::make_pair(buttonInfo, command);
	m_Controls.emplace(controllerKey, controllerCommand);
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

void dae::InputManager::ProcessControllerInput(const XINPUT_STATE& inputState, unsigned controllerId)
{
	for (auto& pair : m_Controls)
	{
		if (pair.first.first != controllerId) continue;
		
		ControllerCommand& cc = pair.second;
		ButtonInfo& bi = cc.first;
		bi.lastValue = bi.pressed;

		const ControllerKey& ck = pair.first;
		ControllerButton button = ck.second;

		cc.first.pressed = inputState.Gamepad.wButtons & static_cast<int>(button);

		const ButtonInfo& buttonInfo = pair.second.first;
		if (buttonInfo.isActive() && pair.second.second != nullptr) pair.second.second->Execute();
	}
}

