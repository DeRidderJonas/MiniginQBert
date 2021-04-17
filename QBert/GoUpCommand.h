#pragma once
#include "Command.h"

namespace QBert
{
	class MovementComponent;

	class GoUpCommand final : public dae::Command
	{
	public:
		GoUpCommand(MovementComponent* pMovementComponent);
		~GoUpCommand() override = default;
		void Execute() override;
	private:
		MovementComponent* m_pMovementComponent;
	};

}
