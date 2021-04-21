#include "MovementComponent.h"

#include "HealthComponent.h"
#include "LevelManager.h"
#include "MoveEvent.h"
#include "PlayableTerrainComponent.h"
#include "TransformComponent.h"

QBert::MovementComponent::MovementComponent(dae::GameObject* pOwner, dae::GameObject* pStandOn)
	: Component(pOwner)
	, m_pStandingOn(pStandOn)
	, m_Subject()
{
	if (!m_pStandingOn) 
	{
		m_pStandingOn = LevelManager::GetInstance().GetSpawnPlatform();
	}

	pOwner->GetComponentOfType<dae::TransformComponent>()->SetPosition(m_pStandingOn->GetComponentOfType<dae::TransformComponent>()->GetPosition());
}

void QBert::MovementComponent::Update()
{
}

void QBert::MovementComponent::Move(Direction direction, bool activatesTerrain, bool revertsTerrain)
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
		GoToSpawningPlatform();
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
	
	if(revertsTerrain)
	{
		auto pTerrainComponent = m_pStandingOn->GetComponentOfType<PlayableTerrainComponent>();
		if (pTerrainComponent) pTerrainComponent->Revert();
	}

	MoveEvent event{ "MOVE", m_pOwner };
	m_Subject.Notify(event);
}

void QBert::MovementComponent::GoToSpawningPlatform()
{
	m_pStandingOn = LevelManager::GetInstance().GetSpawnPlatform();
	m_pOwner->GetComponentOfType<dae::TransformComponent>()->SetPosition(m_pStandingOn->GetComponentOfType<dae::TransformComponent>()->GetPosition());
}

dae::GameObject* QBert::MovementComponent::GetPlatform() const
{
	return m_pStandingOn;
}

void QBert::MovementComponent::AddObserver(dae::Observer* pObserver)
{
	m_Subject.AddObserver(pObserver);
}
