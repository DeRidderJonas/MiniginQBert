#include "SlickSamAIComponent.h"

#include "HealthComponent.h"

QBert::SlickSamAIComponent::SlickSamAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer)
	: AIComponent(pOwner, EnemyType::SlickSam, pPlayer)
{
}

void QBert::SlickSamAIComponent::OnCollisionWithPlayer(dae::GameObject* )
{
	auto pEnemyHealthComponent = m_pOwner->GetComponentOfType<HealthComponent>();
	if (pEnemyHealthComponent) pEnemyHealthComponent->Kill();
}

void QBert::SlickSamAIComponent::OnReachBottom()
{
}

QBert::MovementComponent::Direction QBert::SlickSamAIComponent::GetNextDirectionToGo() const
{
	return rand() % 2 == 0 ? MovementComponent::Direction::DOWN : MovementComponent::Direction::RIGHT;
}
