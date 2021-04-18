#pragma once
#include "TextureComponent.h"

namespace QBert
{
	class TextureLineComponent final : public dae::TextureComponent
	{
	public:
		TextureLineComponent(dae::GameObject* pOwner, const std::string& fileName, int amountInLine = 1);
		~TextureLineComponent() override = default;

		void Render() const override;
		void SetAmountInLine(int amount);
	private:
		int m_AmountInLine;
		glm::vec2 m_TextureDimensions{};
	};
}
