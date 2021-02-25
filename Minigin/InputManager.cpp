#include "MiniginPCH.h"
#include "InputManager.h"

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
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
			ProcessKeyboardInput(e.key.keysym.sym, e.type);
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	return true;
}

void dae::InputManager::Bind(unsigned controllerId, const ControllerButton& button, const std::shared_ptr<Command>& command, const InputState& inputState)
{
	ControllerKey controllerKey = std::make_pair(controllerId, button);
	auto duplicateIt = m_Controls.find(controllerKey);
	if (duplicateIt != m_Controls.end()) throw std::runtime_error("Button was already mapped for controller");

	InputInfo buttonInfo{ inputState };
	InputCommand controllerCommand = std::make_pair(buttonInfo, command);
	m_Controls.emplace(controllerKey, controllerCommand);
}

void dae::InputManager::Bind(const SDL_Keycode& keycode, const std::shared_ptr<Command>& command, InputState inputState)
{
	auto duplicateIt = m_KeyboardControls.find(keycode);
	if (duplicateIt != m_KeyboardControls.end()) throw std::runtime_error("Key was already mapped");

	InputInfo keyInfo{ inputState };
	InputCommand keyCommand = std::make_pair(keyInfo, command);
	m_KeyboardControls.emplace(keycode, keyCommand);
}

bool dae::InputManager::InputInfo::isActive() const
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

//Updates each controller input button
void dae::InputManager::ProcessControllerInput(const XINPUT_STATE& inputState, unsigned controllerId)
{
	for (auto& pair : m_Controls)
	{
		if (pair.first.first != controllerId) continue;
		
		InputCommand& controllerCommand = pair.second;
		InputInfo& inputInfo = controllerCommand.first;
		inputInfo.lastValue = inputInfo.pressed;

		const ControllerKey& controllerKey = pair.first;
		ControllerButton button = controllerKey.second;

		controllerCommand.first.pressed = inputState.Gamepad.wButtons & static_cast<int>(button);

		if (inputInfo.isActive() && pair.second.second != nullptr) pair.second.second->Execute();
	}
}

//updates a single key
void dae::InputManager::ProcessKeyboardInput(const SDL_Keycode& keycode, Uint32 eventType)
{
	auto keycodeIt = m_KeyboardControls.find(keycode);
	if (keycodeIt == m_KeyboardControls.end()) return;

	InputCommand& keyboardCommand = keycodeIt->second;
	InputInfo& inputInfo = keyboardCommand.first;
	inputInfo.lastValue = inputInfo.pressed;

	inputInfo.pressed = eventType == SDL_KEYDOWN;
	
	if (inputInfo.isActive() && keycodeIt->second.second != nullptr) keycodeIt->second.second->Execute();
}

