#include "ScoreEvent.h"

QBert::ScoreEvent::ScoreEvent(const std::string& name, int value)
	: Event(name)
	, m_value(value)
{
}

int QBert::ScoreEvent::GetScoreValue() const
{
	return m_value;
}
