#pragma once
#include "Observer.h"

namespace dae
{
	class Subject final
	{
	public:
		Subject() = default;
		~Subject() = default;
		
		Subject(const Subject&) = delete;
		Subject& operator=(const Subject&) = delete;
		Subject(Subject&&) = delete;
		Subject& operator=(Subject&&) = delete;

		void AddObserver(Observer* pObserver);
		void RemoveObserver(Observer* pObserver);
		void Notify(const Observer::Event& event);
	private:
		std::vector<Observer*> m_Observers{};
	};

}
