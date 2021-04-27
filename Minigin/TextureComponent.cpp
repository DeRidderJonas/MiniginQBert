#include "MiniginPCH.h"
#include "TextureComponent.h"

#include <SDL_render.h>


#include "Renderer.h"
#include "ResourceManager.h"

dae::TextureComponent::TextureComponent(GameObject* pOwner, const std::string& fileName, int renderPriority)
	: RenderComponent(pOwner, renderPriority)
	, m_pTexture(ResourceManager::GetInstance().LoadTexture(fileName))
	, m_pTransform(nullptr)
{
}

dae::TextureComponent::TextureComponent(GameObject* pOwner, int renderPriority)
	: RenderComponent(pOwner, renderPriority)
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

void dae::TextureComponent::Initialize()
{
	m_pTransform = m_pOwner->GetComponentOfType<TransformComponent>();
}

void dae::TextureComponent::SetTexture(const std::shared_ptr<Texture2D>& pTexture)
{
	m_pTexture = pTexture;
}

void dae::TextureComponent::SetTexture(const std::string& fileName)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName);
}

void dae::TextureComponent::SetPosition(float x, float y)
{
	m_pTransform->SetPosition(x, y);
}

glm::vec2 dae::TextureComponent::GetTextureDimensions() const
{
	if (m_pTexture == nullptr) return glm::vec2{};
	
	int w{}, h{};
	int result = SDL_QueryTexture(m_pTexture->GetSDLTexture(), NULL, NULL, &w, &h);
	if (result == 0) return glm::vec2{w, h};
	
	return glm::vec2{};
}

void dae::TextureComponent::SetOverlayColor(const glm::vec3& color)
{
	m_pTexture->SetOverlayColor(color);
}
