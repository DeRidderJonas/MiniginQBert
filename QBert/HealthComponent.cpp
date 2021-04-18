#include "MiniginPCH.h"
#include "HealthComponent.h"

#include "MovementComponent.h"
#include "Observer.h"
#include "ScoreEvent.h"
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
	switch (m_HealthOwner)
	{
	case HealthOwner::QBert:
		{
			//Notify player death
			dae::Event event{"PLAYERDEATH"};
			m_pSubject.Notify(event);

			auto pMovementComponent = m_pOwner->GetComponentOfType<MovementComponent>();
			if (pMovementComponent) pMovementComponent->GoToSpawningPlatform();
		}
		break;
	case HealthOwner::Coily:
		{
			//Notify Coily death
			ScoreEvent event{ "SCORE", 500 };
			m_pSubject.Notify(event);
		}
		break;
	case HealthOwner::SlickSam:
		{
			//Notify Slick or Sam death
			ScoreEvent event{ "SCORE", 300 };
			m_pSubject.Notify(event);
		}
		break;
	}
}

const QBert::HealthComponent::HealthOwner& QBert::HealthComponent::GetHealthOwnerType() const
{
	return m_HealthOwner;
}

void QBert::HealthComponent::AddObserver(dae::Observer* pObserver)
{
	m_pSubject.AddObserver(pObserver);
}
