#include "GoRightCommand.h"

#include "MovementComponent.h"

QBert::GoRightCommand::GoRightCommand(MovementComponent* pMovementComponent)
	: Command()
	, m_pMovementComponent(pMovementComponent)
{
}

void QBert::GoRightCommand::Execute()
{
	m_pMovementComponent->Move(MovementComponent::Direction::RIGHT);
}
