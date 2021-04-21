#include "MoveEvent.h"

QBert::MoveEvent::MoveEvent(const std::string& name, dae::GameObject* pMover)
	: Event(name)
	, m_pMover(pMover)
{
}

dae::GameObject* QBert::MoveEvent::GetMover() const
{
	return m_pMover;
}
