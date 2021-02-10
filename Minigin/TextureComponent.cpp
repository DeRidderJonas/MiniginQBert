#include "MiniginPCH.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::TextureComponent::TextureComponent(const std::shared_ptr<GameObject>& pOwner, const std::string& fileName)
	: RenderComponent(pOwner)
	, m_pTexture{ ResourceManager::GetInstance().LoadTexture(fileName) }
{
}

void dae::TextureComponent::Render() const
{
	std::shared_ptr<GameObject> pOwner{ m_pOwner.lock() };
	const auto& transform = pOwner->GetTransform();
	const auto& pos = transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::TextureComponent::SetTexture(const std::string& fileName)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName);
}
