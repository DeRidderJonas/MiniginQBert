#pragma once
#include <map>
#include <XInput.h>
#include <SDL.h>

#include "Command.h"
#include "Singleton.h"

namespace dae
{
	enum class ControllerButton
	{
		DpadUp = 0x0001,
		DpadDown = 0x0002,
		DpadLeft = 0x0004,
		DpadRight = 0x0008,
		ButtonStart = 0x0010,
		ButtonBack = 0x0020,
		ThumbLeft = 0x0040,
		ThumbRight = 0x0080,
		ShoulderLeft = 0x0100,
		ShoulderRight = 0x0200,
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
	};

	enum class InputState
	{
		pressed,
		released,
		down,
		up
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager() = default;
		bool ProcessInput();
		void Bind(unsigned controllerId, const ControllerButton& button, const std::shared_ptr<Command>& command, const InputState& inputState);
		void Bind(const SDL_Keycode& keycode, const std::shared_ptr<Command>& command, InputState inputState);
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
