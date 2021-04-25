#include "UggWrongWayAIComponent.h"

#include "HealthComponent.h"

QBert::UggWrongWayAIComponent::UggWrongWayAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer)
	: AIComponent(pOwner, EnemyType::UggWrongWay, pPlayer)
{
}

void QBert::UggWrongWayAIComponent::OnCollisionWithPlayer(dae::GameObject* pPlayer)
{
	if (pPlayer) pPlayer->GetComponentOfType<HealthComponent>()->Kill();

	auto pEnemyHealthComponent = m_pOwner->GetComponentOfType<HealthComponent>();
	if (pEnemyHealthComponent) pEnemyHealthComponent->Kill(false);
}

void QBert::UggWrongWayAIComponent::OnReachBottom()
{
}

QBert::MovementComponent::Direction QBert::UggWrongWayAIComponent::GetNextDirectionToGo() const
{
	return rand() % 2 == 0 ? MovementComponent::Direction::DOWN : MovementComponent::Direction::RIGHT;
}
