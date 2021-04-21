#pragma once
#include <Component.h>

namespace QBert
{
	class ScoreComponent;

	class EnemySpawnerComponent final : public dae::Component
	{
	public:
		EnemySpawnerComponent(dae::GameObject* pOwner, ScoreComponent* pScoreComponent, bool spawnInstantly = false);
		~EnemySpawnerComponent() override = default;
		void Update() override;
	private:
		float m_SpawnInterval{ 5.5f };
		float m_timeElapsed;

		ScoreComponent* m_pScoreComponent;
	};

}
