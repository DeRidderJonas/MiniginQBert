#include "MiniginPCH.h"
#include "LivesDisplayComponent.h"


#include "Event.h"
#include "PlayerDeathEvent.h"
#include "TextureLineComponent.h"

QBert::LivesDisplayComponent::LivesDisplayComponent(dae::GameObject* pOwner, TextureLineComponent* pTextureLineComponent, int amountOfLives)
	: Component(pOwner)
	, m_pTextureLineComponent{pTextureLineComponent} 
{
	m_pTextureLineComponent->SetAmountInLine(amountOfLives);
}

void QBert::LivesDisplayComponent::Initialize()
{
}

void QBert::LivesDisplayComponent::Update()
{
}

void QBert::LivesDisplayComponent::OnNotify(const dae::Event& event)
{
	if (event.GetName() == "PLAYERDEATH")
	{
		const PlayerDeathEvent& pEvent = dynamic_cast<const PlayerDeathEvent&>(event);
		m_pTextureLineComponent->SetAmountInLine(pEvent.GetAmountOfLives());
	}
		
}
