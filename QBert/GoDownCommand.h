#pragma once
#include <Command.h>

namespace QBert
{
	class MovementComponent;

	class GoDownCommand : public dae::Command
	{
	public:
		GoDownCommand(MovementComponent* pMovementComponent);
		~GoDownCommand() override = default;
		void Execute() override;
	private:
		MovementComponent* m_pMovementComponent;
	};

}
