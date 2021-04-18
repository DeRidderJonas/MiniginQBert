#include "PlayerDeathEvent.h"

QBert::PlayerDeathEvent::PlayerDeathEvent(const std::string& name, int livesLeft)
	: Event(name)
	, m_amountOfLives(livesLeft)
{
}

int QBert::PlayerDeathEvent::GetAmountOfLives() const
{
	return m_amountOfLives;
}
