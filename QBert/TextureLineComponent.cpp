#include "MiniginPCH.h"
#include "TextureLineComponent.h"

QBert::TextureLineComponent::TextureLineComponent(dae::GameObject* pOwner, const std::string& fileName, int amountInLine)
	: TextureComponent(pOwner, fileName)
	, m_AmountInLine{amountInLine}
{
}

void QBert::TextureLineComponent::Initialize()
{
	m_pTransform = m_pOwner->GetComponentOfType<dae::TransformComponent>();
}

void QBert::TextureLineComponent::Render() const
{
	auto textureDimensions = GetTextureDimensions();
	const auto initialPos = m_pTransform->GetPosition();
	
	for (int i = 0; i < m_AmountInLine; ++i)
	{
		TextureComponent::Render();
		auto pos = m_pTransform->GetPosition();
		pos.x += textureDimensions.x;
		m_pTransform->SetPosition(pos.x, pos.y, pos.z);
	}

	m_pTransform->SetPosition(initialPos.x, initialPos.y, initialPos.z);
}

void QBert::TextureLineComponent::SetAmountInLine(int amount)
{
	m_AmountInLine = amount;
}
