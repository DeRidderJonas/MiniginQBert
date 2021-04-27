#include "UggWrongWayAIComponent.h"

#include "HealthComponent.h"
#include "QBertGameContext.h"
#include "Scene.h"

QBert::UggWrongWayAIComponent::UggWrongWayAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer, bool goLeft)
	: AIComponent(pOwner, EnemyType::UggWrongWay, pPlayer)
	, m_GoingLeft(goLeft)
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
	auto pGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
	auto pMovementComponent = m_pOwner->GetComponentOfType<MovementComponent>();
	if(!pGameContext || !pMovementComponent)
	{
		if(m_GoingLeft)
			return rand() % 2 == 0 ? MovementComponent::Direction::LEFT : MovementComponent::Direction::DOWN;
		else
			return rand() % 2 == 0 ? MovementComponent::Direction::RIGHT : MovementComponent::Direction::UP;
	}

	int row{}, col{};
	pGameContext->GetPlatformForGameObject(pMovementComponent->GetPlatform(), row, col);
	int levelWidth{ pGameContext->GetLevelWidth() };

	if(m_GoingLeft)
	{
		if (row == levelWidth - 1)
			return MovementComponent::Direction::LEFT;
		else
			return rand() % 2 == 0 ? MovementComponent::Direction::LEFT : MovementComponent::Direction::DOWN;
	}
	else
	{
		if (row == levelWidth - 1)
			return MovementComponent::Direction::UP;
		else
			return rand() % 2 == 0 ? MovementComponent::Direction::RIGHT : MovementComponent::Direction::UP;
	}
}
