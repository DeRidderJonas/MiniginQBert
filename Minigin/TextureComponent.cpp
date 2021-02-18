#include "MiniginPCH.h"
#include "TextureComponent.h"

#include "Renderer.h"
#include "ResourceManager.h"

dae::TextureComponent::TextureComponent(const GameObject* pOwner, const std::string& fileName)
	: RenderComponent(pOwner)
	, m_pTexture(ResourceManager::GetInstance().LoadTexture(fileName))
{
	m_pTransform = pOwner->GetComponentOfType<TransformComponent>();
}

dae::TextureComponent::TextureComponent(const GameObject* pOwner)
	: RenderComponent(pOwner)
	, m_pTexture{nullptr}
{
	m_pTransform = pOwner->GetComponentOfType<TransformComponent>();
}

void dae::TextureComponent::Render() const
{
	if (m_pTexture == nullptr) return;

	if(m_pTransform == nullptr)
	{
		throw std::runtime_error("TextureComponent did not have access to TransformComponent");
	}
	
	const auto& pos = m_pTransform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::TextureComponent::SetTexture(const std::shared_ptr<Texture2D>& pTexture)
{
	m_pTexture = pTexture;
}

void dae::TextureComponent::SetTexture(const std::string& fileName)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName);
}
