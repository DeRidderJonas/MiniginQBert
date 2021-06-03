#include "CoilyAIComponent.h"

#include "HealthComponent.h"
#include "QBertGameContext.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "GameTime.h"

QBert::CoilyAIComponent::CoilyAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer)
	: AIComponent(pOwner, EnemyType::Coily, pPlayer)
	, m_HasHatched(false)
	, m_TargetRow()
	, m_TargetX()
	, m_updateTargetRemaining()
{
}

void QBert::CoilyAIComponent::Update()
{
	AIComponent::Update();

	m_updateTargetRemaining -= dae::GameTime::GetInstance().GetDeltaTime();
	if(m_updateTargetRemaining <= 0.f)
	{
		if (!m_pPlayer)
			return;

		auto pPlayerMovement = m_pPlayer->GetComponentOfType<MovementComponent>();
		if (!pPlayerMovement)
			return;

		auto pQBertGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
		if (!pQBertGameContext)
			return;

		auto pTarget = pPlayerMovement->GetPlatform();
		int col{};
		pQBertGameContext->GetPlatformForGameObject(pTarget, m_TargetRow, col);
		m_TargetX = pTarget->GetComponentOfType<dae::TransformComponent>()->GetPosition().x;
		
		m_updateTargetRemaining += m_UpdateTargetInterval;
	}
}

void QBert::CoilyAIComponent::OnReachBottom()
{
	if (m_HasHatched) return;

	m_HasHatched = true;
	m_pOwner->GetComponentOfType<dae::TextureComponent>()->SetTexture("Coily.png");
}

void QBert::CoilyAIComponent::OnCollisionWithPlayer(dae::GameObject* pPlayer)
{
	if(pPlayer) pPlayer->GetComponentOfType<HealthComponent>()->Kill();

	auto pEnemyHealthComponent = m_pOwner->GetComponentOfType<HealthComponent>();
	if(pEnemyHealthComponent) pEnemyHealthComponent->Kill(false);
}

void QBert::CoilyAIComponent::SetTarget(dae::GameObject* pTarget)
{
	auto pQBertGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
	if (!pQBertGameContext)
		return;

	int col{};
	pQBertGameContext->GetPlatformForGameObject(pTarget, m_TargetRow, col);
	m_TargetX = pTarget->GetComponentOfType<dae::TransformComponent>()->GetPosition().x;
	
	m_updateTargetRemaining = m_UpdateTargetInterval;
}

QBert::MovementComponent::Direction QBert::CoilyAIComponent::GetNextDirectionToGo() const
{
	if (m_HasHatched)
	{
		MovementComponent::Direction toGo{ rand() % 2 == 0 ? MovementComponent::Direction::UP : MovementComponent::Direction::LEFT };
		if(m_TargetRow == 0 && m_TargetX == 0)
			return toGo;

		//Path finding
		auto pQBertGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
		if (!pQBertGameContext)
			return toGo;

		auto pPlatform{ m_pOwner->GetComponentOfType<MovementComponent>()->GetPlatform() };
		
		int levelHeight{ pQBertGameContext->GetLevelHeight() };
		
		int coilyRow{}, coilyCol{};
		pQBertGameContext->GetPlatformForGameObject(pPlatform, coilyRow, coilyCol);

		bool playerAbove{ m_TargetRow < coilyRow };
		bool playerBelow{ m_TargetRow > coilyRow };
		float coilyX{ pPlatform->GetComponentOfType<dae::TransformComponent>()->GetPosition().x };
		bool playerLeft{ m_TargetX < coilyX };
		bool playerRight{ m_TargetX > coilyX };

		//Coily below -> Go up
		if (playerAbove)
		{
			if (playerLeft)
				return MovementComponent::Direction::LEFT;
			if (playerRight)
				return MovementComponent::Direction::UP;
			return toGo;
		}

		//Coily above -> Go down
		if(playerBelow)
		{
			if (playerLeft)
				return MovementComponent::Direction::DOWN;
			if (playerRight)
				return MovementComponent::Direction::RIGHT;
			return rand() % 2 == 0 ? MovementComponent::Direction::DOWN : MovementComponent::Direction::RIGHT;
		}
		
		//Coily is on same row -> Go up or down
		if(playerLeft)
		{
			if (m_TargetRow == levelHeight - 1)
				return MovementComponent::Direction::LEFT;

			return rand() % 2 == 0 ? MovementComponent::Direction::LEFT : MovementComponent::Direction::DOWN;
		}
		if(playerRight)
		{
			if (m_TargetRow == levelHeight - 1)
				return MovementComponent::Direction::UP;

			return rand() % 2 == 0 ? MovementComponent::Direction::UP : MovementComponent::Direction::RIGHT;
		}

		//Player is on same tile
		return toGo;
	}
	else
	{
		return rand() % 2 == 0 ? MovementComponent::Direction::DOWN : MovementComponent::Direction::RIGHT;
	}
}
