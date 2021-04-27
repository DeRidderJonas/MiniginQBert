#include "EnemyFactory.h"

#include "CoilyAIComponent.h"
#include "QBertGameContext.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "SlickSamAIComponent.h"
#include "TextureComponent.h"
#include "UggWrongWayAIComponent.h"

dae::GameObject* QBert::EnemyFactory::CreateEnemy(AIComponent::EnemyType type, ScoreComponent* pScoreComponent, QBertGameContext* pGameContext, dae::GameObject* pPlayer)
{
	dae::GameObject* newEnemy = new dae::GameObject();

	//Health component
	HealthComponent::HealthOwner healthOwnerType{ GetHealthOwner(type) };
	auto pHealthComponent = new HealthComponent(newEnemy, healthOwnerType, 1);
	pHealthComponent->AddObserver(pScoreComponent);
	newEnemy->AddComponent(pHealthComponent);

	//Movement component
	bool startLeft{ rand() % 2 == 0 };
	MovementComponent* pMovementComponent = new MovementComponent(newEnemy, GetSpawnPlatform(type, pGameContext, startLeft));
	newEnemy->AddComponent(pMovementComponent);

	//Render component
	dae::TextureComponent* pRenderComponent = new dae::TextureComponent(newEnemy, GetTexturePath(type, startLeft), 1);
	newEnemy->AddComponent(pRenderComponent);

	//AI component
	AIComponent* pAIComponent = CreateAIComponent(type, newEnemy, pPlayer, startLeft);
	newEnemy->AddComponent(pAIComponent);

	return newEnemy;
}

QBert::HealthComponent::HealthOwner QBert::EnemyFactory::GetHealthOwner(AIComponent::EnemyType type)
{
	switch (type)
	{
	default:
	case AIComponent::EnemyType::Coily: 
		return HealthComponent::HealthOwner::Coily;
	case AIComponent::EnemyType::UggWrongWay:
		return HealthComponent::HealthOwner::UggWrongWay;
	case AIComponent::EnemyType::SlickSam: 
		return HealthComponent::HealthOwner::SlickSam;
	}
}

std::string QBert::EnemyFactory::GetTexturePath(AIComponent::EnemyType type, bool startLeft)
{
	switch (type)
	{
	default:
	case AIComponent::EnemyType::Coily: 
		return "CoilyEgg.png";
	case AIComponent::EnemyType::UggWrongWay: 
		return startLeft ? "UggWrongWayGoingRight.png" : "UggWrongWayGoingLeft.png";
	case AIComponent::EnemyType::SlickSam: 
		return "SlickSam.png";
	}
}

QBert::AIComponent* QBert::EnemyFactory::CreateAIComponent(AIComponent::EnemyType type, dae::GameObject* pOwner, dae::GameObject* pPlayer, bool startLeft)
{
	switch (type)
	{
	default:
	case AIComponent::EnemyType::Coily: 
		return new CoilyAIComponent(pOwner, pPlayer);
	case AIComponent::EnemyType::UggWrongWay: 
		return new UggWrongWayAIComponent(pOwner, pPlayer, !startLeft);
	case AIComponent::EnemyType::SlickSam: 
		return new SlickSamAIComponent(pOwner, pPlayer);
	}
}

dae::GameObject* QBert::EnemyFactory::GetSpawnPlatform(AIComponent::EnemyType type, QBertGameContext* pGamecontext, bool startLeft)
{
	dae::GameObject* pSpawnOn{ nullptr };

	int rowMin{}, rowMax{}, colMin{}, colMax{};
	pGamecontext->GetEnemyPlayableRange(rowMin, rowMax, colMin, colMax);
	switch (type)
	{
	case AIComponent::EnemyType::Coily:
		pSpawnOn = startLeft ? pGamecontext->GetPlatform(rowMin+1, colMax - 1) : pGamecontext->GetPlatform(rowMin+1, colMax);
		break;
	case AIComponent::EnemyType::UggWrongWay:
		pSpawnOn = startLeft ? pGamecontext->GetPlatform(rowMax, colMin) : pGamecontext->GetPlatform(rowMax, colMax);
		break;
	case AIComponent::EnemyType::SlickSam:
		pSpawnOn = pGamecontext->GetSpawnPlatform();
		break;
	}
	return pSpawnOn;
}
