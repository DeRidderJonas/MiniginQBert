#pragma once
#include <vector>

#include "AIComponent.h"
#include "GameContext.h"

namespace dae {
	class Scene;
	class GameObject;
}

namespace QBert
{
	class ScoreComponent;
	
	class QBertGameContext : public dae::GameContext
	{
	public:
		QBertGameContext(dae::Scene* pScene);
		virtual ~QBertGameContext() override = default;
		
		void OnAddGameObject(dae::GameObject* pGameObject) override;
		void OnRemoveGameObject(dae::GameObject* pGameObject) override;
		void Update() override;

		void CreateLevel(QBert::ScoreComponent* pScoreComponent);
		void CreatePlayer();
		void Spawn(AIComponent::EnemyType enemyType, ScoreComponent* pScoreComponent = nullptr);
		
		dae::GameObject* GetPlatform(int row, int col) const;
		void GetPlatformForGameObject(dae::GameObject* pToFind, int& row, int& col);
		dae::GameObject* GetSpawnPlatform() const;
		bool IsPlatformOnBottom(dae::GameObject* pGo) const;

		int GetLevelWidth() const;
		void GetEnemyPlayableRange(int& rowMin, int& rowMax, int& colMin, int& colMax) const;
		
		void CheckCollisions();
		void OnPlayerDestroy();
	private:

		const static int m_LevelWidth{ 9 };
		const static int m_LevelHeight{ m_LevelWidth - 2 };
		dae::GameObject* m_GameObjects[m_LevelHeight][m_LevelWidth]{};

		std::vector<dae::GameObject*> m_Enemies{};
		dae::GameObject* m_pPlayer{ nullptr };
	};


}