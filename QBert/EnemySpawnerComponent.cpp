#include "EnemySpawnerComponent.h"

#include <iostream>

#include "GameTime.h"
#include "QBertGameContext.h"
#include "Scene.h"

float randf(float min, float max)
{
	return min + (static_cast<float>(rand()) / (RAND_MAX / (max - min)));
}

QBert::EnemySpawnerComponent::EnemySpawnerComponent(dae::GameObject* pOwner, ScoreComponent* pScoreComponent, bool spawnInstantly)
	: Component(pOwner)
	, m_timeUntillSpawn(spawnInstantly ? 0.f : randf(m_SpawnIntervalMin, m_SpawnIntervalMax))
	, m_pScoreComponent(pScoreComponent)
{
}

void QBert::EnemySpawnerComponent::Update()
{
	m_timeUntillSpawn -= dae::GameTime::GetInstance().GetDeltaTime();

	if(m_timeUntillSpawn <= 0.f)
	{
		auto pQBertGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
		if(pQBertGameContext)
		{
			if(!m_ScriptedSpawns.empty())
			{
				pQBertGameContext->Spawn(m_ScriptedSpawns.front(), m_pScoreComponent);
				m_ScriptedSpawns.pop();
			}
			else
			{
				pQBertGameContext->Spawn(static_cast<AIComponent::EnemyType>(rand() % 3), m_pScoreComponent);
			}
		}
		
		m_timeUntillSpawn += randf(m_SpawnIntervalMin, m_SpawnIntervalMax);
	}
}

void QBert::EnemySpawnerComponent::Initialize()
{
}

void QBert::EnemySpawnerComponent::SetScriptedSpawns(const std::queue<AIComponent::EnemyType>& scriptedSpawns)
{
	m_ScriptedSpawns = scriptedSpawns;
}
