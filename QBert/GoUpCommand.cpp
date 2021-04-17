#include "GoUpCommand.h"

#include "MovementComponent.h"

QBert::GoUpCommand::GoUpCommand(MovementComponent* pMovementComponent)
	: m_pMovementComponent(pMovementComponent)
{
}

void QBert::GoUpCommand::Execute()
{
	m_pMovementComponent->Move(MovementComponent::Direction::UP);
}
