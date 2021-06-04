#pragma once
#include <Component.h>
#include <queue>

#include "AIComponent.h"

namespace QBert
{
	class ScoreComponent;

	class EnemySpawnerComponent final : public dae::Component
	{
	public:
		EnemySpawnerComponent(dae::GameObject* pOwner, ScoreComponent* pScoreComponent, bool spawnInstantly = false);
		~EnemySpawnerComponent() override = default;

		void Update() override;
		void Initialize() override;

		void SetScriptedSpawns(const std::queue<AIComponent::EnemyType>& scriptedSpawns);
	private:
		float m_SpawnIntervalMin{ 4.5f };
		float m_SpawnIntervalMax{ 7.5f };
		float m_timeUntillSpawn;

		//https://gamefaqs.gamespot.com/nes/587540-qbert/faqs/64992
		std::queue<AIComponent::EnemyType> m_ScriptedSpawns;

		ScoreComponent* m_pScoreComponent;
	};

}
