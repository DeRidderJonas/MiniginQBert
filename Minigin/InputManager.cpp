#include "MiniginPCH.h"
#include "InputManager.h"

#include <XInput.h>
#include <SDL.h>

//PIMPL
namespace dae
{
	class InputManagerImpl final
	{
	public:
		bool ProcessInput();
		void Bind(unsigned controllerId, const ControllerButton& button, const std::shared_ptr<Command>& command, const InputState& inputState);
		void Bind(const SDL_Keycode& keycode, const std::shared_ptr<Command>& command, InputState inputState);

		void Unbind(const SDL_Keycode& keycode);
		void Unbind(unsigned controlledId, const ControllerButton& button);

		InputManagerImpl() = default;
		~InputManagerImpl() = default;
		InputManagerImpl(const InputManagerImpl&) = delete;
		InputManagerImpl& operator=(const InputManagerImpl&) = delete;
		InputManagerImpl(InputManagerImpl&&) = delete;
		InputManagerImpl& operator=(InputManagerImpl&&) = delete;
	private:
		struct InputInfo
		{
		public:
			InputInfo(const InputState& InputState)
				: pressed{ false }, lastValue{ false }, stateRequired{ InputState }{};
			bool pressed;
			bool lastValue;
			InputState stateRequired;

			bool isActive() const;
		};

		void ProcessControllerInput(const XINPUT_STATE& inputState, unsigned controllerId);
		void ProcessKeyboardInput(const SDL_Keycode& keycode, Uint32 eventType);

		// MAPPING
		using InputCommand = std::pair<InputInfo, std::shared_ptr<Command>>;

		// CONTROLLER MAPPING
		//unsigned = controller id
		using ControllerKey = std::pair<unsigned, ControllerButton>;
		using ControllerCommandMap = std::map<ControllerKey, InputCommand>;
		ControllerCommandMap m_Controls{};

		// KEYBOARD MAPPING
		using KeyboardCommandMap = std::map<SDL_Keycode, InputCommand>;
		KeyboardCommandMap m_KeyboardControls{};
	};
}

bool dae::InputManagerImpl::ProcessInput()
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

void dae::InputManagerImpl::Bind(unsigned controllerId, const ControllerButton& button, const std::shared_ptr<Command>& command, const InputState& inputState)
{
	ControllerKey controllerKey = std::make_pair(controllerId, button);
	auto duplicateIt = m_Controls.find(controllerKey);
	if (duplicateIt != m_Controls.end()) throw std::runtime_error("Button was already mapped for controller");

	InputInfo buttonInfo{ inputState };
	InputCommand controllerCommand = std::make_pair(buttonInfo, command);
	m_Controls.emplace(controllerKey, controllerCommand);
}

void dae::InputManagerImpl::Bind(const SDL_Keycode& keycode, const std::shared_ptr<Command>& command, InputState inputState)
{
	auto duplicateIt = m_KeyboardControls.find(keycode);
	if (duplicateIt != m_KeyboardControls.end()) throw std::runtime_error("Key was already mapped");

	InputInfo keyInfo{ inputState };
	InputCommand keyCommand = std::make_pair(keyInfo, command);
	m_KeyboardControls.emplace(keycode, keyCommand);
}

void dae::InputManagerImpl::Unbind(const SDL_Keycode& keycode)
{
	auto findIt = m_KeyboardControls.find(keycode);
	if (findIt == m_KeyboardControls.end())
		return;

	m_KeyboardControls.erase(findIt);
}

void dae::InputManagerImpl::Unbind(unsigned controlledId, const ControllerButton& button)
{
	ControllerKey controllerKey = std::make_pair(controlledId, button);
	auto findIt = m_Controls.find(controllerKey);
	if (findIt == m_Controls.end())
		return;

	m_Controls.erase(findIt);
}

bool dae::InputManagerImpl::InputInfo::isActive() const
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
void dae::InputManagerImpl::ProcessControllerInput(const XINPUT_STATE& inputState, unsigned controllerId)
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
void dae::InputManagerImpl::ProcessKeyboardInput(const SDL_Keycode& keycode, Uint32 eventType)
{
	auto keycodeIt = m_KeyboardControls.find(keycode);
	if (keycodeIt == m_KeyboardControls.end()) return;

	InputCommand& keyboardCommand = keycodeIt->second;
	InputInfo& inputInfo = keyboardCommand.first;
	inputInfo.lastValue = inputInfo.pressed;

	inputInfo.pressed = eventType == SDL_KEYDOWN;
	
	if (inputInfo.isActive() && keycodeIt->second.second != nullptr) keycodeIt->second.second->Execute();
}

dae::InputManager::InputManager()
{
	m_pimpl = new InputManagerImpl();
}

dae::InputManager::~InputManager()
{
	delete m_pimpl;
}

bool dae::InputManager::ProcessInput()
{
	return m_pimpl->ProcessInput();
}

void dae::InputManager::Bind(unsigned controllerId, const ControllerButton& button,
	const std::shared_ptr<Command>& command, const InputState& inputState)
{
	m_pimpl->Bind(controllerId, button, command, inputState);
}

void dae::InputManager::Bind(int keycode, const std::shared_ptr<Command>& command,
	InputState inputState)
{
	m_pimpl->Bind(keycode, command, inputState);
}

void dae::InputManager::Unbind(int keycode)
{
	m_pimpl->Unbind(keycode);
}

void dae::InputManager::Unbind(unsigned controlledId, const ControllerButton& button)
{
	m_pimpl->Unbind(controlledId, button);
}
