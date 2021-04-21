#include "MiniginPCH.h"
#include "EnemyDeathEvent.h"

QBert::EnemyDeathEvent::EnemyDeathEvent(const std::string& name, int scoreValue, dae::GameObject* pEnemy)
	: Event{name}
	, m_value{scoreValue}
	, m_pEnemy(pEnemy)
{
}

dae::GameObject* QBert::EnemyDeathEvent::GetEnemy() const
{
	return m_pEnemy;
}

int QBert::EnemyDeathEvent::GetScoreValue() const
{
	return m_value;
}
