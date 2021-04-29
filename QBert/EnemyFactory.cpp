#include "EnemyFactory.h"

#include "CoilyAIComponent.h"
#include "HealthComponent.h"
#include "QBertGameContext.h"
#include "ScoreComponent.h"
#include "SlickSamAIComponent.h"
#include "TextureComponent.h"
#include "UggWrongWayAIComponent.h"

QBert::HealthComponent::HealthOwner GetHealthOwner(QBert::AIComponent::EnemyType type);
std::string GetTexturePath(QBert::AIComponent::EnemyType type, bool startLeft);
QBert::AIComponent* CreateAIComponent(QBert::AIComponent::EnemyType type, dae::GameObject* pOwner, dae::GameObject* pPlayer, bool startLeft);
dae::GameObject* GetSpawnPlatform(QBert::AIComponent::EnemyType type, QBert::QBertGameContext* pGamecontext, bool startLeft);

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

QBert::HealthComponent::HealthOwner GetHealthOwner(QBert::AIComponent::EnemyType type)
{
	switch (type)
	{
	default:
	case QBert::AIComponent::EnemyType::Coily: 
		return QBert::HealthComponent::HealthOwner::Coily;
	case QBert::AIComponent::EnemyType::UggWrongWay:
		return QBert::HealthComponent::HealthOwner::UggWrongWay;
	case QBert::AIComponent::EnemyType::SlickSam: 
		return QBert::HealthComponent::HealthOwner::SlickSam;
	}
}

std::string GetTexturePath(QBert::AIComponent::EnemyType type, bool startLeft)
{
	switch (type)
	{
	default:
	case QBert::AIComponent::EnemyType::Coily: 
		return "CoilyEgg.png";
	case QBert::AIComponent::EnemyType::UggWrongWay: 
		return startLeft ? "UggWrongWayGoingRight.png" : "UggWrongWayGoingLeft.png";
	case QBert::AIComponent::EnemyType::SlickSam: 
		return "SlickSam.png";
	}
}

QBert::AIComponent* CreateAIComponent(QBert::AIComponent::EnemyType type, dae::GameObject* pOwner, dae::GameObject* pPlayer, bool startLeft)
{
	switch (type)
	{
	default:
	case QBert::AIComponent::EnemyType::Coily: 
		return new QBert::CoilyAIComponent(pOwner, pPlayer);
	case QBert::AIComponent::EnemyType::UggWrongWay: 
		return new QBert::UggWrongWayAIComponent(pOwner, pPlayer, !startLeft);
	case QBert::AIComponent::EnemyType::SlickSam: 
		return new QBert::SlickSamAIComponent(pOwner, pPlayer);
	}
}

dae::GameObject* GetSpawnPlatform(QBert::AIComponent::EnemyType type, QBert::QBertGameContext* pGamecontext, bool startLeft)
{
	dae::GameObject* pSpawnOn{ nullptr };

	int rowMin{}, rowMax{}, colMin{}, colMax{};
	pGamecontext->GetEnemyPlayableRange(rowMin, rowMax, colMin, colMax);
	switch (type)
	{
	case QBert::AIComponent::EnemyType::Coily:
		pSpawnOn = startLeft ? pGamecontext->GetPlatform(rowMin+1, colMax - 1) : pGamecontext->GetPlatform(rowMin+1, colMax);
		break;
	case QBert::AIComponent::EnemyType::UggWrongWay:
		pSpawnOn = startLeft ? pGamecontext->GetPlatform(rowMax, colMin) : pGamecontext->GetPlatform(rowMax, colMax);
		break;
	case QBert::AIComponent::EnemyType::SlickSam:
		pSpawnOn = pGamecontext->GetSpawnPlatform();
		break;
	}
	return pSpawnOn;
}
