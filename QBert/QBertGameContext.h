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
		enum class GameMode
		{
			Single,
			Coop,
			Versus
		};

		QBertGameContext(dae::Scene* pScene, GameMode gameMode = GameMode::Single);
		virtual ~QBertGameContext() override = default;
		
		void OnAddGameObject(dae::GameObject* pGameObject) override;
		void OnRemoveGameObject(dae::GameObject* pGameObject) override;
		void Update() override;

		bool CreateLevel(QBert::ScoreComponent* pScoreComponent);
		void CreatePlayer();
		void Spawn(AIComponent::EnemyType enemyType, ScoreComponent* pScoreComponent = nullptr);

		std::vector<dae::GameObject*>& GetEnemies();
		
		dae::GameObject* GetPlatform(int row, int col) const;
		void GetPlatformForGameObject(dae::GameObject* pToFind, int& row, int& col);
		dae::GameObject* GetSpawnPlatform(bool isPlayer = false, bool playerTwo = false) const;
		bool IsPlatformOnBottom(dae::GameObject* pGo) const;

		int GetLevelWidth() const;
		int GetLevelHeight() const;
		void GetEnemyPlayableRange(int& rowMin, int& rowMax, int& colMin, int& colMax) const;
		
		void CheckCollisions();
		void CheckPlatforms();
		
		void OnPlayerDestroy();
	private:
		void CheckCollisionForPlayer(dae::GameObject* pPlayer);

		void GoToNextLevel();
		void KillEnemies();
		void DestroyLevel();

		GameMode m_gameMode;

		const static int m_LevelWidth{ 9 };
		const static int m_LevelHeight{ m_LevelWidth - 2 };

		dae::GameObject* m_PlayableGrid[m_LevelHeight][m_LevelWidth]{};
		int m_CurrentLevel{ 1 };
		
		std::vector<dae::GameObject*> m_Enemies{};
		dae::GameObject* m_pPlayer{ nullptr };
		dae::GameObject* m_pPlayerTwo{ nullptr };
		ScoreComponent* m_pScoreComponent{ nullptr };
	};


}