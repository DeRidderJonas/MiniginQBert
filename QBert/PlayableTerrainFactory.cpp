#include "PlayableTerrainFactory.h"

#include "TextureComponent.h"
#include "ScoreComponent.h"

dae::GameObject* QBert::PlayableTerrainFactory::CreatePlatform(PlayableTerrainComponent::TerrainType type,
                                                               ScoreComponent* pScoreComponent, int row, int col, float terrainWidth, float startX, float startY)
{
	auto go = new dae::GameObject();
	auto pPlatformTexture = new dae::TextureComponent(go);
	go->AddComponent(pPlatformTexture);

	auto pPlayable = new PlayableTerrainComponent(go, type, pPlatformTexture);
	pPlayable->AddObserver(pScoreComponent);
	go->AddComponent(pPlayable);

	float x{ sqrtf(3) * static_cast<float>(col) + sqrtf(3) / 2.f * static_cast<float>(row) };
	float y{ 3.f / 2.f * static_cast<float>(row) };
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(terrainWidth * x + startX, terrainWidth * y + startY);

	return go;
}

dae::GameObject* QBert::PlayableTerrainFactory::CreatePlatform(char type, ScoreComponent* pScoreComponent, int row,
	int col, float terrainWidth, float startX, float startY)
{
	switch (type)
	{
	case '0':
	default:
		return nullptr;
	case '1':
		return CreatePlatform(PlayableTerrainComponent::TerrainType::Normal, pScoreComponent, row, col, terrainWidth, startX, startY);
	case '2':
		return CreatePlatform(PlayableTerrainComponent::TerrainType::Double, pScoreComponent, row, col, terrainWidth, startX, startY);
	case '3':
		return CreatePlatform(PlayableTerrainComponent::TerrainType::Reverting, pScoreComponent, row, col, terrainWidth, startX, startY);
	case '4':
		return CreatePlatform(PlayableTerrainComponent::TerrainType::Disc, pScoreComponent, row, col, terrainWidth, startX, startY);
	}
}
