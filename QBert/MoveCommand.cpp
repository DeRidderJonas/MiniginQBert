#include "MoveCommand.h"

QBert::MoveCommand::MoveCommand(MovementComponent* pMovementComponent, MovementComponent::Direction direction,
	bool activatesTerrain, bool revertsTerrain)
	: Command()
	, m_pMovementComponent(pMovementComponent)
	, m_Direction(direction)
	, m_ActivatesTerrain(activatesTerrain)
	, m_RevertsTerrain(revertsTerrain)
{
}

void QBert::MoveCommand::Execute()
{
	if (m_pMovementComponent) m_pMovementComponent->Move(m_Direction, m_ActivatesTerrain, m_RevertsTerrain);
}
