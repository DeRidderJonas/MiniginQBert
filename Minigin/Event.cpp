#include "MiniginPCH.h"
#include "Event.h"

dae::Event::Event(const std::string& name)
	: m_Name{name}
{
}

const std::string& dae::Event::GetName() const
{
	return m_Name;
}
