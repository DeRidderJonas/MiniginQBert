#include "PlayableTerrainComponent.h"

#include "TextureComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/gtx/compatibility.hpp>

#include "CoilyAIComponent.h"
#include "MovementComponent.h"
#include "QBertGameContext.h"
#include "Scene.h"
#include "ScoreEvent.h"
#pragma warning(pop)

QBert::PlayableTerrainComponent::PlayableTerrainComponent(dae::GameObject* pOwner, TerrainType type, dae::TextureComponent* pPlatform)
	: Component(pOwner)
	, m_pPlatform(pPlatform)
	, m_Type(type)
	, m_Subject()
	, m_StepsNeeded(INT_MAX)
{	
}

void QBert::PlayableTerrainComponent::Initialize()
{
	if (m_Type == TerrainType::Disc)
		m_pPlatform->SetTexture("disc.png");
	else
		m_pPlatform->SetTexture("Hex.png");

	Revert();
}

void QBert::PlayableTerrainComponent::Update()
{
}

void QBert::PlayableTerrainComponent::Activate(dae::GameObject* pActivatedBy)
{
	bool awardPoints{ false };
	switch (m_Type)
	{
	case TerrainType::Normal:
	case TerrainType::Double:
		if (m_StepsNeeded > 0)
		{
			m_StepsNeeded--;
			awardPoints = true;
		}
		break;
	case TerrainType::Disc:
		if (m_StepsNeeded > 0)
		{
			m_StepsNeeded--;
			if(pActivatedBy)
			{
				auto pMovementComponent = pActivatedBy->GetComponentOfType<MovementComponent>();
				if (pMovementComponent) pMovementComponent->GoToSpawningPlatform();

				m_pOwner->Destroy();

				auto pQBertGameContext = dynamic_cast<QBertGameContext*>(m_pOwner->GetScene()->GetGameContext());
				if(pQBertGameContext)
				{
					auto& enemies{ pQBertGameContext->GetEnemies() };
					for(auto pEnemy : enemies)
					{
						if (pEnemy->ShouldBeDestroyed())
							continue;
						
						auto pCoilyAI = pEnemy->GetComponentOfType<CoilyAIComponent>();
						if(pCoilyAI)
							pCoilyAI->SetTarget(m_pOwner);
					}
				}
			}
		}
		break;
	case TerrainType::Reverting:
		if (m_StepsNeeded > 0)
		{
			m_StepsNeeded--;
			awardPoints = true;
		}
		else m_StepsNeeded++;
		break;
	}

	SetTextureComponentColors();

	if(awardPoints)
	{
		ScoreEvent event{ "SCORE", 25 };
		m_Subject.Notify(event);
	}
}

void QBert::PlayableTerrainComponent::Revert()
{
	switch (m_Type)
	{
	case TerrainType::Normal:
	case TerrainType::Reverting:
	case TerrainType::Disc:
		m_StepsNeeded = 1;
		break;
	case TerrainType::Double: 
		m_StepsNeeded = 2;
		break;
	}

	SetTextureComponentColors();
}

bool QBert::PlayableTerrainComponent::IsFullyActivated() const
{
	if (m_Type == TerrainType::Disc) return true;

	return m_StepsNeeded == 0;
}

QBert::PlayableTerrainComponent::TerrainType QBert::PlayableTerrainComponent::GetType() const
{
	return m_Type;
}

void QBert::PlayableTerrainComponent::AddObserver(dae::Observer* pObserver)
{
	m_Subject.AddObserver(pObserver);
}

void QBert::PlayableTerrainComponent::SetTextureComponentColors() const
{
	if (m_Type == TerrainType::Disc) return;
	
	glm::vec3 color{ m_ColorActive };
	if (m_StepsNeeded == 1) color = m_ColorInactive;
	else if (m_StepsNeeded == 2) color = m_ColorIntermediate;
	
	m_pPlatform->SetOverlayColor(color);
}
