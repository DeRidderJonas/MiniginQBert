#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "Time.h"

dae::FPSComponent::FPSComponent(GameObject* pOwner, RenderComponent* pTextComponent)
	: Component(pOwner)
	, m_pTextComponent{pTextComponent}
{
}

void dae::FPSComponent::Update()
{
	int fps = Time::GetInstance().GetFPS();
	m_pTextComponent->SetText(std::to_string(fps) + " FPS");
}
