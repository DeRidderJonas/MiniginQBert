#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "GameTime.h"

dae::FPSComponent::FPSComponent(GameObject* pOwner, TextComponent* pRenderComponent)
	: Component(pOwner)
	, m_pTextComponent{pRenderComponent}
	, m_FPS{}
{
}

void dae::FPSComponent::Update()
{
	int fps = GameTime::GetInstance().GetFPS();
	if(fps != m_FPS)
	{
		m_FPS = fps;
		m_pTextComponent->SetText(std::to_string(fps) + " FPS");
	}
}
