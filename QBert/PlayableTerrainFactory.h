#pragma once
#include "PlayableTerrainComponent.h"

namespace dae {
	class GameObject;
}

namespace QBert
{
	class PlayableTerrainFactory final
	{
	public:
		static dae::GameObject* CreatePlatform(PlayableTerrainComponent::TerrainType type, ScoreComponent* pScoreComponent, int row, int col, float terrainWidth, float startX, float startY);
		static dae::GameObject* CreatePlatform(unsigned char type, ScoreComponent* pScoreComponent, int row, int col, float terrainWidth, float startX, float startY);
	};

}
