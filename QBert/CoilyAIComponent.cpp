#include "CoilyAIComponent.h"

#include "TextureComponent.h"

QBert::CoilyAIComponent::CoilyAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer)
	: AIComponent(pOwner, EnemyType::Coily, pPlayer)
	, m_HasHatched(false)
{
}

void QBert::CoilyAIComponent::OnReachBottom()
{
	if (m_HasHatched) return;

	m_HasHatched = true;
	m_pOwner->GetComponentOfType<dae::TextureComponent>()->SetTexture("Coily.png");
}

QBert::MovementComponent::Direction QBert::CoilyAIComponent::GetNextDirectionToGo() const
{
	if (m_HasHatched)
	{
		//TODO: Path finding
		MovementComponent::Direction toGo{ rand() % 2 == 0 ? MovementComponent::Direction::UP : MovementComponent::Direction::LEFT };
		return toGo;
	}
	else
	{
		return rand() % 2 == 0 ? MovementComponent::Direction::DOWN : MovementComponent::Direction::RIGHT;
	}
}
