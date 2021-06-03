#include "MovementComponent.h"

#include "AIComponent.h"
#include "HealthComponent.h"
#include "PlayableTerrainComponent.h"
#include "QBertGameContext.h"
#include "Scene.h"
#include "TransformComponent.h"

QBert::MovementComponent::MovementComponent(dae::GameObject* pOwner, dae::GameObject* pStandOn)
	: Component(pOwner)
	, m_pStandingOn(pStandOn)
	, m_Subject()
{
	
}

void QBert::MovementComponent::Initialize()
{
	if (!m_pStandingOn)
	{
		auto pGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
		if (pGameContext) m_pStandingOn = pGameContext->GetSpawnPlatform();
	}

	if (m_pStandingOn) m_pOwner->GetComponentOfType<dae::TransformComponent>()->SetPosition(m_pStandingOn->GetComponentOfType<dae::TransformComponent>()->GetPosition());
}

void QBert::MovementComponent::Update()
{
}

void QBert::MovementComponent::Move(Direction direction, bool activatesTerrain, bool revertsTerrain, bool canStandOnDisc)
{
	int row{ -1 }, col{ -1 };
	auto pGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
	pGameContext->GetPlatformForGameObject(m_pStandingOn, row, col);
	dae::GameObject* pNext{ nullptr };
	switch (direction)
	{
	case Direction::UP:
		pNext = pGameContext->GetPlatform(row-1, col+1);
		break;
	case Direction::DOWN:
		pNext = pGameContext->GetPlatform(row+1, col-1);
		break;
	case Direction::LEFT:
		pNext = pGameContext->GetPlatform(row-1, col);
		break;
	case Direction::RIGHT:
		pNext = pGameContext->GetPlatform(row+1, col);
		break;
	}

	if(pNext == nullptr)
	{
		FallOffGrid();
		return;
	}

	auto pTerrainComponent = pNext->GetComponentOfType<PlayableTerrainComponent>();
	if(pTerrainComponent->GetType() == PlayableTerrainComponent::TerrainType::Disc && !canStandOnDisc)
	{
		FallOffGrid();
		return;
	}

	auto ownerTransform = m_pOwner->GetComponentOfType<dae::TransformComponent>();
	ownerTransform->SetPosition(pNext->GetComponentOfType<dae::TransformComponent>()->GetPosition());
	m_pStandingOn = pNext;

	if(activatesTerrain)
	{
		if (pTerrainComponent) pTerrainComponent->Activate(m_pOwner);
	}
	
	if(revertsTerrain)
	{
		if (pTerrainComponent) pTerrainComponent->Revert();
	}

	if(pGameContext->IsPlatformOnBottom(m_pStandingOn))
	{
		auto aiComponent = m_pOwner->GetComponentOfType<AIComponent>();
		if (aiComponent) aiComponent->OnReachBottom();
	}
}

void QBert::MovementComponent::GoToSpawningPlatform(bool isPlayer, bool isPlayerTwo)
{
	auto pGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
	if (pGameContext)
	{
		m_pStandingOn = pGameContext->GetSpawnPlatform(isPlayer, isPlayerTwo);
		m_pOwner->GetComponentOfType<dae::TransformComponent>()->SetPosition(m_pStandingOn->GetComponentOfType<dae::TransformComponent>()->GetPosition());
	}
}

dae::GameObject* QBert::MovementComponent::GetPlatform() const
{
	return m_pStandingOn;
}

void QBert::MovementComponent::AddObserver(dae::Observer* pObserver)
{
	m_Subject.AddObserver(pObserver);
}

void QBert::MovementComponent::FallOffGrid()
{
	//Fell into the void
	auto pHealthComponent = m_pOwner->GetComponentOfType<HealthComponent>();
	if (pHealthComponent) pHealthComponent->Kill();
	GoToSpawningPlatform();
}
