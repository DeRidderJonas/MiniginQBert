#include "EnemyManager.h"

#include <iostream>

#include "EnemyDeathEvent.h"
#include "HealthComponent.h"
#include "MoveEvent.h"
#include "MovementComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "TextureComponent.h"

void QBert::EnemyManager::SetPlayer(dae::GameObject* pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->GetComponentOfType<MovementComponent>()->AddObserver(this);
}

void QBert::EnemyManager::Spawn(AIComponent::EnemyType enemyType, ScoreComponent* pScoreComponent, dae::GameObject* pSpawnOn)
{
	dae::Scene& scene = dae::SceneManager::GetInstance().GetScene("QBert");

	dae::GameObject* newEnemy = new dae::GameObject();

	//Health component
	HealthComponent::HealthOwner healthOwnerType{ enemyType == AIComponent::EnemyType::Coily ? HealthComponent::HealthOwner::Coily :
		(enemyType == AIComponent::EnemyType::Ugg || enemyType == AIComponent::EnemyType::WrongWay) ? HealthComponent::HealthOwner::UggWrongWay : HealthComponent::HealthOwner::SlickSam };
	auto pHealthComponent = new HealthComponent(newEnemy, healthOwnerType, 1);
	pHealthComponent->AddObserver(pScoreComponent);
	pHealthComponent->AddObserver(this);
	newEnemy->AddComponent(pHealthComponent);

	//Movement component
	MovementComponent* pMovementComponent = new MovementComponent(newEnemy, pSpawnOn);
	newEnemy->AddComponent(pMovementComponent);
	pMovementComponent->AddObserver(this);

	//Render component
	dae::TextureComponent* pRenderComponent = new dae::TextureComponent(newEnemy, "Coily.png"); //TODO: Fix png based on AIComponent::EnemyType
	newEnemy->AddComponent(pRenderComponent);

	//AI component
	AIComponent* pAIComponent = new AIComponent(newEnemy, enemyType, pMovementComponent, m_pPlayer);
	newEnemy->AddComponent(pAIComponent);

	m_Enemies.push_back(newEnemy);
	scene.Add(newEnemy);
}

void QBert::EnemyManager::OnNotify(const dae::Event& event)
{
	if(event.GetName() == "ENEMYDEATH")
	{
		const EnemyDeathEvent& scoreEvent = dynamic_cast<const EnemyDeathEvent&>(event);
		m_Enemies.erase(std::remove_if(m_Enemies.begin(), m_Enemies.end(), 
			[&scoreEvent](dae::GameObject* pGo) {return pGo == scoreEvent.GetEnemy(); }), 
			m_Enemies.end());
	}
	else if(event.GetName() == "MOVE" && m_pPlayer)
	{
		//const MoveEvent& moveEvent = dynamic_cast<const MoveEvent&>(event);
		auto pPlayerMovementComponent = m_pPlayer->GetComponentOfType<MovementComponent>();
		
		auto enemyIt = std::find_if(m_Enemies.begin(), m_Enemies.end(), [&pPlayerMovementComponent](dae::GameObject* pGo)
			{
				auto pMovementComponent = pGo->GetComponentOfType<MovementComponent>();
				return pPlayerMovementComponent->GetPlatform() == pMovementComponent->GetPlatform();
			});

		if(enemyIt != m_Enemies.end())
		{
			//Player and enemy on same platform
			dae::GameObject* pEnemy = *enemyIt;
			auto pAIComponent = pEnemy->GetComponentOfType<AIComponent>();
			auto pEnemyHealthComponent = pEnemy->GetComponentOfType<HealthComponent>();
			switch (pAIComponent->GetType())
			{
			case AIComponent::EnemyType::Coily: 
			case AIComponent::EnemyType::Ugg: 
			case AIComponent::EnemyType::WrongWay:
				m_pPlayer->GetComponentOfType<HealthComponent>()->Kill();
				pEnemyHealthComponent->Kill(false);
				break;
			case AIComponent::EnemyType::Slick: 
			case AIComponent::EnemyType::Sam:
				pEnemyHealthComponent->Kill();
				break;
			}
		}
	}
}
