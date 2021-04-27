#include "MiniginPCH.h"


#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"

dae::RenderComponent::RenderComponent(GameObject* pOwner, int renderPriority)
	: Component{pOwner}
	, m_RenderPriority(renderPriority)
{
}

void dae::RenderComponent::Update()
{
}

void dae::RenderComponent::SetRenderPriority(int value)
{
	m_RenderPriority = value;
}

int dae::RenderComponent::GetRenderPriority() const
{
	return m_RenderPriority;
}
