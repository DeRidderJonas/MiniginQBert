#pragma once
#include "AIComponent.h"
#include "Observer.h"
#include "Singleton.h"

namespace QBert
{
	class ScoreComponent;

	class EnemyManager final : public dae::Singleton<EnemyManager>, dae::Observer
	{
	public:
		void SetPlayer(dae::GameObject* pPlayer);
		void Spawn(AIComponent::EnemyType enemyType, ScoreComponent* pScoreComponent = nullptr, dae::GameObject* pSpawnOn = nullptr);
		void OnNotify(const dae::Event& event) override;
	private:
		friend class Singleton<EnemyManager>;
		EnemyManager(){};

		std::vector<dae::GameObject*> m_Enemies{};
		dae::GameObject* m_pPlayer{ nullptr };
	};

}
