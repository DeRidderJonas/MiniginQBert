#pragma once
#include <string>

#include "Event.h"

namespace QBert
{
	class PlayerDeathEvent final : public dae::Event
	{
	public:
		PlayerDeathEvent(const std::string& name, int livesLeft);
		~PlayerDeathEvent() override = default;

		int GetAmountOfLives() const;
	private:
		int m_amountOfLives;
	};
}

