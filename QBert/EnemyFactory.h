#pragma once

#include "AIComponent.h"

namespace dae {
	class GameObject;
}

namespace QBert
{
	class QBertGameContext;
	class ScoreComponent;

	class EnemyFactory final
	{
	public:
		static dae::GameObject* CreateEnemy(AIComponent::EnemyType type, ScoreComponent* pScoreComponent, QBertGameContext* pGameContext, dae::GameObject* pPlayer = nullptr, bool useAI = true);
	};

}
