#pragma once
#include "GameContext.h"

namespace dae {
	class TextComponent;
}

namespace QBert
{
	class GameOverGameContext final : public dae::GameContext
	{
	public:
		GameOverGameContext(dae::Scene* pScene);
		~GameOverGameContext() override;
		
		void OnAddGameObject(dae::GameObject* pGameObject) override;
		void OnRemoveGameObject(dae::GameObject* pGameObject) override;
		void Update() override;

		void SetScore(int score);
	private:
		int m_score;
		dae::TextComponent* m_pTextComponent;
		dae::TextComponent* m_pTextScoreComponent;
	};
}
