#include "MiniginPCH.h"
#include "ScoreComponent.h"

#include "TextComponent.h"

QBert::ScoreComponent::ScoreComponent(dae::GameObject* pOwner, dae::TextComponent* pTextComponent)
	: Component{pOwner}
	, m_pTextComponent{pTextComponent}
	, m_Score{}
{
	pTextComponent->SetText("00000");
}

void QBert::ScoreComponent::Update()
{
}

void QBert::ScoreComponent::OnNotify(const dae::Observer::Event& event)
{
	if(event.name == "SCORE")
	{
		m_Score += int(event.value);
		char buffer[10]{};
		sprintf_s(buffer, "%05d", m_Score);
		m_pTextComponent->SetText(buffer);
	}
}
