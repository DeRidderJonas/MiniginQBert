#include "UggWrongWayAIComponent.h"

QBert::UggWrongWayAIComponent::UggWrongWayAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer)
	: AIComponent(pOwner, EnemyType::UggWrongWay, pPlayer)
{
}

void QBert::UggWrongWayAIComponent::OnReachBottom()
{
}

QBert::MovementComponent::Direction QBert::UggWrongWayAIComponent::GetNextDirectionToGo() const
{
	return rand() % 2 == 0 ? MovementComponent::Direction::DOWN : MovementComponent::Direction::RIGHT;
}
