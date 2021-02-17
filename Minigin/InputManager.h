#pragma once
#include <map>
#include <XInput.h>

#include "Command.h"
#include "Singleton.h"

namespace dae
{
	enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,
		ButtonStart,
		ButtonBack,
		ShoulderRight,
		ShoulderLeft,
		DpadLeft,
		DpadRight,
		DpadUp,
		DpadDown,
		ThumbLeft,
		ThumbRight,
		Last
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
		InputManager();
		bool ProcessInput();
		void Bind(ControllerButton button, const std::shared_ptr<Command>& command, InputState inputState);
	private:
		XINPUT_STATE m_CurrentState{};

		struct ButtonInfo
		{
		public:
			bool pressed;
			bool lastValue;
			InputState stateRequired;

			bool isActive() const;
		};

		void ProcessControllerInput();
		
		//unsigned = controller id
		using ControllerKey = std::pair<unsigned, ControllerButton>;
		using ControllerCommand = std::pair<ButtonInfo, std::shared_ptr<Command>>;
		using ControllerCommandMap = std::map<ControllerKey, ControllerCommand>;
		ControllerCommandMap m_Controls{};
	};

}
