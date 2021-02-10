#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "TransformComponent.h"
#include "Renderer.h"

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, const std::shared_ptr<Font>& font)
	: RenderComponent{ pOwner }
	, m_Text{text}
	, m_Font{font}
	, m_pTexture{nullptr}
	, m_HasChanged{true}
{
}

dae::TextComponent::~TextComponent()
{
	delete m_pTexture;
}

void dae::TextComponent::Update()
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

		if (m_pTexture != nullptr) delete m_pTexture;
		m_pTexture = new Texture2D(texture);

		m_HasChanged = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_pTexture == nullptr) return;
	
	const TransformComponent* pTransform = m_pOwner->GetComponentOfType<TransformComponent>();
	if (pTransform == nullptr) return;

	const auto& pos = pTransform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_HasChanged = true;
}
