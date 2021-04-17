#pragma once
#include "Event.h"

namespace QBert
{
	class ScoreEvent final: public dae::Event
	{
	public:
		ScoreEvent(const std::string& name, int value);

		int GetValue() const;
	private:
		int m_value;
	};
}
