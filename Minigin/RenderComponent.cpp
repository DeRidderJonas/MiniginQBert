#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"

dae::RenderComponent::RenderComponent(const GameObject* pOwner, const std::string& fileName)
	: Component{pOwner}
	, m_pTexture{ResourceManager::GetInstance().LoadTexture(fileName)}
	, m_HasChanged{false}
	, m_Text{}
	, m_Font{nullptr}
{
}

dae::RenderComponent::RenderComponent(const GameObject* pOwner, const std::string& text, const std::shared_ptr<Font>& font)
	: Component{pOwner}
	, m_pTexture{nullptr}
	, m_HasChanged{true}
	, m_Text{text}
	, m_Font{font}
{
}

void dae::RenderComponent::Update()
{
	if (m_HasChanged)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTexture = std::make_shared<Texture2D>(texture);
		m_HasChanged = false;
	}
}

void dae::RenderComponent::Render() const
{
	if (m_pTexture == nullptr) return;

	const TransformComponent* pTransform = m_pOwner->GetComponentOfType<TransformComponent>();
	if (pTransform == nullptr) return;

	const auto& pos = pTransform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::RenderComponent::SetTexture(const std::string& fileName)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName);
}

void dae::RenderComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_HasChanged = true;
}
