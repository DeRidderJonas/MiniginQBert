#include "MiniginPCH.h"
#include "LivesComponent.h"


#include "Event.h"
#include "TextureLineComponent.h"

QBert::LivesComponent::LivesComponent(dae::GameObject* pOwner, TextureLineComponent* pTextureLineComponent, int amountOfLives)
	: Component(pOwner)
	, m_AmountOfLives{amountOfLives}
	, m_pTextureLineComponent{pTextureLineComponent} 
{
	m_pTextureLineComponent->SetAmountInLine(amountOfLives);
}

void QBert::LivesComponent::Update()
{
}

void QBert::LivesComponent::OnNotify(const dae::Event& event)
{
	if (event.GetName() == "PLAYERDEATH")
	{
		m_AmountOfLives--;
		m_pTextureLineComponent->SetAmountInLine(m_AmountOfLives);
	}
		
}
