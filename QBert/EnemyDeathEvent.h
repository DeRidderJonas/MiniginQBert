#pragma once
#include "Event.h"

namespace dae {
	class GameObject;
}

namespace QBert
{
	class EnemyDeathEvent final: public dae::Event
	{
	public:
		EnemyDeathEvent(const std::string& name, int scoreValue, dae::GameObject* pEnemy);

		dae::GameObject* GetEnemy() const;
		int GetScoreValue() const;
	private:
		int m_value;
		dae::GameObject* m_pEnemy;
	};
}
