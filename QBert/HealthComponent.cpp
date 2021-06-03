#include "MiniginPCH.h"
#include "HealthComponent.h"

#include "MovementComponent.h"
#include "Observer.h"
#include "PlayerDeathEvent.h"
#include "EnemyDeathEvent.h"
#include "QBertGameContext.h"
#include "Scene.h"
#include "Subject.h"

QBert::HealthComponent::HealthComponent(dae::GameObject* pOwner, HealthOwner healthOwner, int amountOfLives)
	: Component{pOwner}
	, m_HealthOwner{healthOwner}
	, m_pSubject{}
	, m_AmountOfLives(amountOfLives)
{
}

void QBert::HealthComponent::Initialize()
{
}

void QBert::HealthComponent::Update()
{
}

void QBert::HealthComponent::Kill(bool awardPoints)
{
	m_AmountOfLives--;
	
	switch (m_HealthOwner)
	{
	case HealthOwner::QBert:
		{
			//Notify player death
			PlayerDeathEvent event{"PLAYERDEATH", m_AmountOfLives};
			m_pSubject.Notify(event);
		}
		break;
	case HealthOwner::Coily:
		{
			//Notify Coily death
			EnemyDeathEvent event{ "ENEMYDEATH", awardPoints ? 500 : 0, m_pOwner };
			m_pSubject.Notify(event);
		}
		break;
	case HealthOwner::SlickSam:
		{
			//Notify Slick or Sam death
			EnemyDeathEvent event{ "ENEMYDEATH", awardPoints ? 300 : 0, m_pOwner };
			m_pSubject.Notify(event);
		}
		break;
	
	case HealthOwner::UggWrongWay:
		{
			//Notify Ugg or WrongWay death
			EnemyDeathEvent event{ "ENEMYDEATH", 0, m_pOwner };
			m_pSubject.Notify(event);
		}
		break;
	}

	if(m_AmountOfLives <= 0)
	{
		m_pOwner->Destroy();

		if(m_HealthOwner == HealthOwner::QBert)
		{
			auto pGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
			if (pGameContext)
				pGameContext->OnPlayerDestroy();
		}
	}
}

const QBert::HealthComponent::HealthOwner& QBert::HealthComponent::GetHealthOwnerType() const
{
	return m_HealthOwner;
}

bool QBert::HealthComponent::IsDead() const
{
	return m_AmountOfLives <= 0;
}

void QBert::HealthComponent::AddObserver(dae::Observer* pObserver)
{
	m_pSubject.AddObserver(pObserver);
}
