#include "MiniginPCH.h"
#include "Subject.h"

void dae::Subject::AddObserver(Observer* pObserver)
{
	m_Observers.push_back(pObserver);
}

void dae::Subject::RemoveObserver(Observer* pObserver)
{
	auto findIt = std::find(m_Observers.begin(), m_Observers.end(), pObserver);
	if (findIt == m_Observers.end()) return;

	m_Observers.erase(findIt, m_Observers.end());
}

void dae::Subject::Notify(const Observer::Event& event)
{
	for(Observer* pObserver : m_Observers)
	{
		pObserver->OnNotify(event);
	}
}
