#pragma once
#include "Command.h"
#include "MovementComponent.h"

namespace QBert
{
	class MoveCommand final : public dae::Command
	{
	public:
		MoveCommand(MovementComponent* pMovementComponent, MovementComponent::Direction direction, bool activatesTerrain = false);
		~MoveCommand() override = default;
		void Execute() override;
	private:
		MovementComponent* m_pMovementComponent;
		MovementComponent::Direction m_Direction;
		bool m_activatesTerrain;
	};

}
