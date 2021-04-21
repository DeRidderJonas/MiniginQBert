#include "EnemySpawnerComponent.h"

#include "EnemyManager.h"
#include "GameTime.h"

QBert::EnemySpawnerComponent::EnemySpawnerComponent(dae::GameObject* pOwner, ScoreComponent* pScoreComponent, bool spawnInstantly)
	: Component(pOwner)
	, m_timeElapsed(spawnInstantly ? m_SpawnInterval : 0.f)
	, m_pScoreComponent(pScoreComponent)
{
}

void QBert::EnemySpawnerComponent::Update()
{
	m_timeElapsed += dae::GameTime::GetInstance().GetDeltaTime();

	if(m_timeElapsed > m_SpawnInterval)
	{
		EnemyManager::GetInstance().Spawn(AIComponent::EnemyType::Coily, m_pScoreComponent);
		m_timeElapsed = 0.f;
	}
}
