#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "Time.h"

dae::FPSComponent::FPSComponent(GameObject* pOwner, RenderComponent* pRenderComponent)
	: Component(pOwner)
	, m_pRenderComponent{pRenderComponent}
{
}

void dae::FPSComponent::Update()
{
	int fps = Time::GetInstance().GetFPS();
	m_pRenderComponent->SetText(std::to_string(fps) + " FPS");
}
