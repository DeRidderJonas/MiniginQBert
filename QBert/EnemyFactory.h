#pragma once
#include <string>

#include "AIComponent.h"
#include "HealthComponent.h"

namespace dae {
	class GameObject;
}

namespace QBert
{
	class ScoreComponent;

	class EnemyFactory
	{
	public:
		static dae::GameObject* CreateEnemy(AIComponent::EnemyType type, ScoreComponent* pScoreComponent, dae::GameObject* pPlayer = nullptr, dae::GameObject* pStandOn = nullptr);
	private:
		static HealthComponent::HealthOwner GetHealthOwner(AIComponent::EnemyType type);
		static std::string GetTexturePath(AIComponent::EnemyType type);
		static AIComponent* CreateAIComponent(AIComponent::EnemyType type, dae::GameObject* pOwner, dae::GameObject* pPlayer);
	};

}
