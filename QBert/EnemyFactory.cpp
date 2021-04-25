#include "EnemyFactory.h"

#include "CoilyAIComponent.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "SlickSamAIComponent.h"
#include "TextureComponent.h"
#include "UggWrongWayAIComponent.h"

dae::GameObject* QBert::EnemyFactory::CreateEnemy(AIComponent::EnemyType type, ScoreComponent* pScoreComponent,
                                                  dae::GameObject* pPlayer, dae::GameObject* pStandOn)
{
	dae::GameObject* newEnemy = new dae::GameObject();

	//Health component
	HealthComponent::HealthOwner healthOwnerType{ GetHealthOwner(type) };
	auto pHealthComponent = new HealthComponent(newEnemy, healthOwnerType, 1);
	pHealthComponent->AddObserver(pScoreComponent);
	newEnemy->AddComponent(pHealthComponent);

	//Movement component
	MovementComponent* pMovementComponent = new MovementComponent(newEnemy, pStandOn);
	newEnemy->AddComponent(pMovementComponent);

	//Render component
	dae::TextureComponent* pRenderComponent = new dae::TextureComponent(newEnemy, GetTexturePath(type));
	newEnemy->AddComponent(pRenderComponent);

	//AI component
	AIComponent* pAIComponent = CreateAIComponent(type, newEnemy, pPlayer);
	newEnemy->AddComponent(pAIComponent);

	return newEnemy;
}

QBert::HealthComponent::HealthOwner QBert::EnemyFactory::GetHealthOwner(AIComponent::EnemyType type)
{
	switch (type)
	{
	case AIComponent::EnemyType::Coily: 
		return HealthComponent::HealthOwner::Coily;
	case AIComponent::EnemyType::UggWrongWay:
		return HealthComponent::HealthOwner::UggWrongWay;
	case AIComponent::EnemyType::SlickSam: 
	default:
		return HealthComponent::HealthOwner::SlickSam;
	}
}

std::string QBert::EnemyFactory::GetTexturePath(AIComponent::EnemyType type)
{
	switch (type)
	{
	case AIComponent::EnemyType::Coily: 
		return "CoilyEgg.png";
	case AIComponent::EnemyType::UggWrongWay: 
		return "UggWrongWay.png";
	case AIComponent::EnemyType::SlickSam: 
	default:
		return "SlickSam.png";
	}
}

QBert::AIComponent* QBert::EnemyFactory::CreateAIComponent(AIComponent::EnemyType type, dae::GameObject* pOwner, dae::GameObject* pPlayer)
{
	switch (type)
	{
	case AIComponent::EnemyType::Coily: 
		return new CoilyAIComponent(pOwner, pPlayer);
	case AIComponent::EnemyType::UggWrongWay: 
		return new UggWrongWayAIComponent(pOwner, pPlayer);
	case AIComponent::EnemyType::SlickSam: 
	default:
		return new SlickSamAIComponent(pOwner, pPlayer);
	}
}
