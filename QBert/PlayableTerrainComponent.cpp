#include "PlayableTerrainComponent.h"

#include "TextureComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/gtx/compatibility.hpp>
#pragma warning(pop)

QBert::PlayableTerrainComponent::PlayableTerrainComponent(dae::GameObject* pOwner, TerrainType type, dae::TextureComponent* pTop, dae::TextureComponent* pLeft, dae::TextureComponent* pRight)
	: Component(pOwner)
	, m_pTop(pTop)
	, m_pLeft(pLeft)
	, m_pRight(pRight)
	, m_Type(type)
{
	switch (type)
	{
	case TerrainType::Normal: 
	case TerrainType::Reverting: 
		m_StepsNeeded = 1;
		break;
	case TerrainType::Double: 
		m_StepsNeeded = 2;
		break;
	}
	
	m_pTop->SetTexture("HexTop.png");
	m_pLeft->SetTexture("HexLeft.png");
	m_pRight->SetTexture("HexRight.png");
	SetTextureComponentColors();
}

void QBert::PlayableTerrainComponent::Update()
{
}

void QBert::PlayableTerrainComponent::Activate()
{
	switch (m_Type)
	{
	case TerrainType::Normal:
	case TerrainType::Double:
		if (m_StepsNeeded > 0) m_StepsNeeded--;
		break;
	case TerrainType::Reverting:
		if (m_StepsNeeded > 0) m_StepsNeeded--;
		else m_StepsNeeded++;
		break;
	}

	SetTextureComponentColors();
}

void QBert::PlayableTerrainComponent::SetTextureComponentColors() const
{
	glm::vec3 color{ m_ColorActive };
	if (m_StepsNeeded == 1) color = m_ColorInactive;
	else if (m_StepsNeeded == 2) color = m_ColorIntermediate;
	
	m_pTop->SetOverlayColor(color);

	auto darkened{ color - 150.f * glm::vec3{1,1,1} };
	darkened.x = glm::clamp(darkened.x, 0.f, 255.f);
	darkened.y = glm::clamp(darkened.y, 0.f, 255.f);
	darkened.z = glm::clamp(darkened.z, 0.f, 255.f);
	m_pLeft->SetOverlayColor(darkened);
	
	auto extraDarkened{ color - 200.f * glm::vec3{1,1,1} };
	extraDarkened.x = glm::clamp(extraDarkened.x, 0.f, 255.f);
	extraDarkened.y = glm::clamp(extraDarkened.y, 0.f, 255.f);
	extraDarkened.z = glm::clamp(extraDarkened.z, 0.f, 255.f);
	m_pRight->SetOverlayColor(extraDarkened);
	
}
