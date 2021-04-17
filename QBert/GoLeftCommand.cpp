#include "GoLeftCommand.h"

#include "MovementComponent.h"

QBert::GoLeftCommand::GoLeftCommand(MovementComponent* pMovementComponent)
	: Command()
	, m_pMovementComponent(pMovementComponent)
{
}

void QBert::GoLeftCommand::Execute()
{
	m_pMovementComponent->Move(MovementComponent::Direction::LEFT);
}
