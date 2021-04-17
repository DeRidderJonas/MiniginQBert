#include "GoDownCommand.h"

#include "MovementComponent.h"

QBert::GoDownCommand::GoDownCommand(MovementComponent* pMovementComponent)
	: Command()
	, m_pMovementComponent(pMovementComponent)
{
}

void QBert::GoDownCommand::Execute()
{
	m_pMovementComponent->Move(MovementComponent::Direction::DOWN);
}
