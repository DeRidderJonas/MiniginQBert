#include "MovementComponent.h"

#include "HealthComponent.h"
#include "LevelManager.h"
#include "PlayableTerrainComponent.h"
#include "TransformComponent.h"

QBert::MovementComponent::MovementComponent(dae::GameObject* pOwner, dae::GameObject* pStandOn)
	: Component(pOwner)
	, m_pStandingOn(pStandOn)
{
	if (m_pStandingOn) pOwner->GetComponentOfType<dae::TransformComponent>()->SetPosition(m_pStandingOn->GetComponentOfType<dae::TransformComponent>()->GetPosition());
}

void QBert::MovementComponent::Update()
{
}

void QBert::MovementComponent::Move(Direction direction, bool activatesTerrain)
{
	int row{ -1 }, col{ -1 };
	auto& levelManager = LevelManager::GetInstance();
	levelManager.GetPositionForGameObject(m_pStandingOn, row, col);
	dae::GameObject* pNext{ nullptr };
	switch (direction)
	{
	case Direction::UP:
		pNext = levelManager.GetGameObject(row-1, col+1);
		break;
	case Direction::DOWN:
		pNext = levelManager.GetGameObject(row+1, col-1);
		break;
	case Direction::LEFT:
		pNext = levelManager.GetGameObject(row-1, col);
		break;
	case Direction::RIGHT:
		pNext = levelManager.GetGameObject(row+1, col);
		break;
	}

	if(pNext == nullptr)
	{
		//Fell into the void
		auto pHealthComponent = m_pOwner->GetComponentOfType<HealthComponent>();
		if (pHealthComponent) pHealthComponent->Kill();
		return;
	}

	auto ownerTransform = m_pOwner->GetComponentOfType<dae::TransformComponent>();
	ownerTransform->SetPosition(pNext->GetComponentOfType<dae::TransformComponent>()->GetPosition());
	m_pStandingOn = pNext;
	if(activatesTerrain)
	{
		auto pTerrainComponent = m_pStandingOn->GetComponentOfType<PlayableTerrainComponent>();
		if (pTerrainComponent) pTerrainComponent->Activate();
	}
}
