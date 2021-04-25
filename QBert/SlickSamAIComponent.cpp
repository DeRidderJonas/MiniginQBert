#include "SlickSamAIComponent.h"

QBert::SlickSamAIComponent::SlickSamAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer)
	: AIComponent(pOwner, EnemyType::SlickSam, pPlayer)
{
}

void QBert::SlickSamAIComponent::OnReachBottom()
{
}

QBert::MovementComponent::Direction QBert::SlickSamAIComponent::GetNextDirectionToGo() const
{
	return rand() % 2 == 0 ? MovementComponent::Direction::DOWN : MovementComponent::Direction::RIGHT;
}
