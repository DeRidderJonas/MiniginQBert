#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "TextComponent.h"

#include "Renderer.h"

dae::TextComponent::TextComponent(GameObject* pOwner, TextureComponent* pTextureComponet, const std::string& text, const std::shared_ptr<Font>& font)
	: Component(pOwner)
	, m_Text{ text }
	, m_Font{font}
	, m_pTextureComponent{ pTextureComponet }
	, m_HasChanged{true}
{
}

void dae::TextComponent::Update()
{
	if (m_HasChanged)
	{
		if(m_pTextureComponent == nullptr)
		{
			throw std::runtime_error("TextComponent did not have access to TextureComponent");
		}
		
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
		m_pTextureComponent->SetTexture(std::make_shared<Texture2D>(texture));
		m_HasChanged = false;
	}
}

void dae::TextComponent::Initialize()
{
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_HasChanged = true;
}
