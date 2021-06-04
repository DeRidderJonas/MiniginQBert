#include "MovementComponent.h"

#include "AIComponent.h"
#include "GameTime.h"
#include "HealthComponent.h"
#include "PlayableTerrainComponent.h"
#include "QBertGameContext.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "TransformComponent.h"

QBert::MovementComponent::MovementComponent(dae::GameObject* pOwner, dae::GameObject* pStandOn)
	: Component(pOwner)
	, m_pStandingOn(pStandOn)
	, m_Subject()
	, m_movedTime()
	, m_moveFrom()
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
	if (m_pOwner->ShouldBeDestroyed() || m_movedTime >= m_moveTime || m_pStandingOn == nullptr)
		return;

	m_movedTime += dae::GameTime::GetInstance().GetDeltaTime();
	
	float percentage{ m_movedTime / m_moveTime };
	auto destPos = m_pStandingOn->GetComponentOfType<dae::TransformComponent>()->GetPosition();
	glm::vec2 destination{ destPos.x, destPos.y };
	glm::vec2 direction{ destination - m_moveFrom };

	glm::vec2 interpolatedPos{ m_moveFrom + percentage * direction };
	m_pOwner->GetComponentOfType<dae::TransformComponent>()->SetPosition(interpolatedPos.x, interpolatedPos.y);
}

void QBert::MovementComponent::Move(Direction direction, bool activatesTerrain, bool revertsTerrain, bool canStandOnDisc)
{
	if (m_movedTime < m_moveTime)
		return;
	
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
		if(!activatesTerrain)
			dae::ServiceLocator::GetSoundSystem().Play("../Data/fall.wav");
		return;
	}

	auto currPos = m_pOwner->GetComponentOfType<dae::TransformComponent>()->GetPosition();
	m_moveFrom.x = currPos.x;
	m_moveFrom.y = currPos.y;
	m_movedTime = 0.f;

	auto pTerrainComponent = pNext->GetComponentOfType<PlayableTerrainComponent>();
	if(pTerrainComponent->GetType() == PlayableTerrainComponent::TerrainType::Disc && !canStandOnDisc)
	{
		FallOffGrid();
		return;
	}

	m_pStandingOn = pNext;

	if(activatesTerrain)
	{
		if (pTerrainComponent) pTerrainComponent->Activate(m_pOwner);
		dae::ServiceLocator::GetSoundSystem().Play("../Data/jump.wav", 0.5f);
	}
	else
	{
		dae::ServiceLocator::GetSoundSystem().Play("../Data/jump2.wav", 0.5f);
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
	if (pHealthComponent) pHealthComponent->Kill(pHealthComponent->GetHealthOwnerType() != HealthComponent::HealthOwner::SlickSam);
	GoToSpawningPlatform();
}
