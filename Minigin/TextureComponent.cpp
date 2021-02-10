#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"

dae::TextureComponent::TextureComponent(const GameObject* pOwner, const std::string& fileName)
	: RenderComponent(pOwner)
	, m_pTexture{ ResourceManager::GetInstance().LoadTexture(fileName) }
{
}

void dae::TextureComponent::Render() const
{
	const auto& transform = m_pOwner->GetComponentOfType<TransformComponent>();
	const auto& pos = transform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::TextureComponent::SetTexture(const std::string& fileName)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName);
}
