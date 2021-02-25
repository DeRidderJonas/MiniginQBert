#include "MiniginPCH.h"
#include "HealthComponent.h"

#include "Observer.h"
#include "Subject.h"

QBert::HealthComponent::HealthComponent(dae::GameObject* pOwner, HealthOwner healthOwner)
	: Component{pOwner}
	, m_HealthOwner{healthOwner}
	, m_pSubject{}
{
}

void QBert::HealthComponent::Update()
{
}

void QBert::HealthComponent::Kill()
{
	dae::Observer::Event event{};
	switch (m_HealthOwner)
	{
	case HealthOwner::QBert: 
		//Notify player death
		event.name = "PLAYERDEATH";
		break;
	case HealthOwner::Coily:
		//Notify Coily death
		event.name = "SCORE";
		event.value = 500;
		break;
	case HealthOwner::SlickSam: 
		//Notify Slick or Sam death
		event.name = "SCORE";
		event.value = 300;
		break;
	}
	m_pSubject.Notify(event);
}

const QBert::HealthComponent::HealthOwner& QBert::HealthComponent::GetHealthOwnerType() const
{
	return m_HealthOwner;
}

void QBert::HealthComponent::AddObserver(dae::Observer* pObserver)
{
	m_pSubject.AddObserver(pObserver);
}
