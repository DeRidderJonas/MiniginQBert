#pragma once
#include "RenderComponent.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	class TextComponent final : public RenderComponent
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, const std::shared_ptr<Font>& font);
		virtual ~TextComponent();

		void Update() override;
		void Render() const override;
		
		void SetText(const std::string& text);
	private:
		bool m_HasChanged;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		Texture2D* m_pTexture;
	};
}

