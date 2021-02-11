#pragma once
#include "Component.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(const GameObject* pOwner, const std::string& fileName);
		RenderComponent(const GameObject* pOwner, const std::string& text, const std::shared_ptr<Font>& font);
		virtual ~RenderComponent() = default;

		void Update() override;
		void Render() const;

		void SetTexture(const std::string& fileName);
		void SetText(const std::string& text);
	private:
		std::shared_ptr<Texture2D> m_pTexture;

		bool m_HasChanged;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
	};

}
