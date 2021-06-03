#pragma once
#include "Component.h"
#include "Font.h"
#include "TextureComponent.h"

namespace dae
{
	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* pOwner, TextureComponent* pTextureComponent, const std::string& text, const std::shared_ptr<Font>& font);

		void Update() override;
		void Initialize() override;

		void SetText(const std::string& text);
	private:
		TextureComponent* m_pTextureComponent;
		
		bool m_HasChanged;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
	};

}
