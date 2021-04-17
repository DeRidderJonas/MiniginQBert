#pragma once
#include "Command.h"

namespace QBert
{
	class MovementComponent;

	class GoRightCommand : public dae::Command
	{
	public:
		GoRightCommand(MovementComponent* pMovementComponent);
		~GoRightCommand() override = default;
		void Execute() override;
	private:
		MovementComponent* m_pMovementComponent;
	};

}
