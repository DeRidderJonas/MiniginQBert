#pragma once
#include <Command.h>

namespace QBert
{
	class MovementComponent;

	class GoLeftCommand : public dae::Command
	{
	public:
		GoLeftCommand(MovementComponent* pMovementComponent);
		~GoLeftCommand() override = default;
		void Execute() override;
	private:
		MovementComponent* m_pMovementComponent;
	};

}