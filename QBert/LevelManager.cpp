#include "MiniginPCH.h"
#include "LevelManager.h"

#include "GameObject.h"
#include "PlayableTerrainComponent.h"
#include "Scene.h"
#include "TextureComponent.h"

void QBert::LevelManager::CreateLevel(dae::Scene& scene)
{
	float HexWidth{ 20.f };
	
	for (int r = 0; r < m_width; ++r)
	{
		for (int q = 0; q < m_width; ++q)
		{
			if (q < m_width - r - 1)
				continue;

			auto go  = new dae::GameObject();
			auto pTopTexture = new dae::TextureComponent(go);
			auto pLeftTexture = new dae::TextureComponent(go);
			auto pRightTexture = new dae::TextureComponent(go);
			go->AddComponent(pTopTexture);
			go->AddComponent(pLeftTexture);
			go->AddComponent(pRightTexture);
			auto play = new PlayableTerrainComponent(go, pTopTexture, pLeftTexture, pRightTexture);
			go->AddComponent(play);
			
			m_GameObjects[r][q] = go;
			float x{ sqrtf(3) * static_cast<float>(q) + sqrtf(3) / 2.f * static_cast<float>(r) };
			float y{ 3.f / 2.f * static_cast<float>(r) };
			go->GetComponentOfType<dae::TransformComponent>()->SetPosition(HexWidth * x + 80.f, HexWidth * y + 110.f);
			scene.Add(go);
		}
	}
}

dae::GameObject* QBert::LevelManager::GetGameObject(int row, int col) const
{
	if (row >= m_width || row < 0 || col >= m_width || col < 0) return nullptr;
	
	return m_GameObjects[row][col];
}

void QBert::LevelManager::GetPositionForGameObject(dae::GameObject* pToFind, int& row, int& col)
{
	for (int r = 0; r < m_width; ++r)
	{
		for (int c = 0; c < m_width; ++c)
		{
			if(m_GameObjects[r][c] == pToFind)
			{
				row = r;
				col = c;
				return;
			}
		}
	}
}

QBert::LevelManager::LevelManager()
{

}
