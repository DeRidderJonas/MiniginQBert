#pragma once
#include <string>

#include "AIComponent.h"
#include "HealthComponent.h"

namespace dae {
	class GameObject;
}

namespace QBert
{
	class QBertGameContext;
	class ScoreComponent;

	class EnemyFactory
	{
	public:
		static dae::GameObject* CreateEnemy(AIComponent::EnemyType type, ScoreComponent* pScoreComponent, QBertGameContext* pGameContext, dae::GameObject* pPlayer = nullptr);
	private:
		static HealthComponent::HealthOwner GetHealthOwner(AIComponent::EnemyType type);
		static std::string GetTexturePath(AIComponent::EnemyType type, bool startLeft);
		static AIComponent* CreateAIComponent(AIComponent::EnemyType type, dae::GameObject* pOwner, dae::GameObject* pPlayer, bool startLeft);
		static dae::GameObject* GetSpawnPlatform(AIComponent::EnemyType type, QBertGameContext* pGamecontext, bool startLeft);
	};

}
