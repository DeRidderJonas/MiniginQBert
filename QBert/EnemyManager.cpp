#include "EnemyManager.h"

#include "EnemyDeathEvent.h"
#include "HealthComponent.h"
#include "MovementComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "TextureComponent.h"

void QBert::EnemyManager::SetPlayer(dae::GameObject* pPlayer)
{
	m_pPlayer = pPlayer;
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

	//Render component
	dae::TextureComponent* pRenderComponent = new dae::TextureComponent(newEnemy, "Coily.png"); //TODO: Fix png based on AIComponent::EnemyType
	newEnemy->AddComponent(pRenderComponent);

	//AI component
	AIComponent* pAIComponent = new AIComponent(newEnemy, enemyType, pMovementComponent, m_pPlayer);
	newEnemy->AddComponent(pAIComponent);

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
}
