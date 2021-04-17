#include "PlayableTerrainComponent.h"

#include "TextureComponent.h"

QBert::PlayableTerrainComponent::PlayableTerrainComponent(dae::GameObject* pOwner, dae::TextureComponent* pTop, dae::TextureComponent* pLeft, dae::TextureComponent* pRight)
	: Component(pOwner)
	, m_pTop(pTop)
	, m_pLeft(pLeft)
	, m_pRight(pRight)
{
	m_pTop->SetTexture("HexTop.png");
	m_pTop->SetOverlayColor({ 255,255,0 });
	m_pLeft->SetTexture("HexLeft.png");
	m_pLeft->SetOverlayColor({ 0,0,255 });
	m_pRight->SetTexture("HexRight.png");
	m_pRight->SetOverlayColor({0,0,128});
}

void QBert::PlayableTerrainComponent::Update()
{
}
