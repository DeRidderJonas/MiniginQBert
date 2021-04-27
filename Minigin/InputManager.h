#pragma once
#include <map>

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
		InputManager();
		~InputManager();
		bool ProcessInput();
		void Bind(unsigned controllerId, const ControllerButton& button, const std::shared_ptr<Command>& command, const InputState& inputState);
		void Bind(int keycode, const std::shared_ptr<Command>& command, InputState inputState);

		void Unbind(int keycode);
		void Unbind(unsigned controlledId, const ControllerButton& button);
	private:
		class InputManagerImpl;
		InputManagerImpl* m_pimpl{ nullptr };
	};

}
