#include "MoveCommand.h"

QBert::MoveCommand::MoveCommand(MovementComponent* pMovementComponent, MovementComponent::Direction direction,
	bool activatesTerrain)
	: Command()
	, m_pMovementComponent(pMovementComponent)
	, m_Direction(direction)
	, m_activatesTerrain(activatesTerrain)
{
}

void QBert::MoveCommand::Execute()
{
	if (m_pMovementComponent) m_pMovementComponent->Move(m_Direction, m_activatesTerrain);
}
