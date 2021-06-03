#include "EnemySpawnerComponent.h"

#include <iostream>

#include "GameTime.h"
#include "QBertGameContext.h"
#include "Scene.h"

QBert::EnemySpawnerComponent::EnemySpawnerComponent(dae::GameObject* pOwner, ScoreComponent* pScoreComponent, bool spawnInstantly)
	: Component(pOwner)
	, m_timeElapsed(spawnInstantly ? m_SpawnInterval : 0.f)
	, m_pScoreComponent(pScoreComponent)
{
}

void QBert::EnemySpawnerComponent::Update()
{
	m_timeElapsed += dae::GameTime::GetInstance().GetDeltaTime();

	//TODO: Instead of complete random spawns, make it Scripted random: "After X seconds spawn Coily, then after X spawn Ugg..."
	if(m_timeElapsed > m_SpawnInterval)
	{
		auto pQBertGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
		if(pQBertGameContext)
		{
			pQBertGameContext->Spawn(AIComponent::EnemyType(rand() % 3), m_pScoreComponent);
			//pQBertGameContext->Spawn(AIComponent::EnemyType::Coily, m_pScoreComponent);
		}
		m_timeElapsed = 0.f;
	}
}

void QBert::EnemySpawnerComponent::Initialize()
{
}
