#include "MiniginPCH.h"
#include "Texture2D.h"
#include <SDL.h>

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

void dae::Texture2D::SetOverlayColor(const glm::vec3& color)
{
	int result = SDL_SetTextureColorMod(m_Texture, Uint8(color.r), Uint8(color.g), Uint8(color.b));
	if(result != 0)
	{
		std::cout << "Texture2D::SetOverlayColor: " << SDL_GetError() << std::endl;
	}
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

dae::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}
