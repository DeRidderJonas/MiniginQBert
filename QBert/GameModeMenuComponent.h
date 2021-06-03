#pragma once
#include "QBertGameContext.h"
#include "RenderComponent.h"

namespace QBert
{
	class GameModeMenuComponent final : public dae::RenderComponent
	{
	public:
		GameModeMenuComponent(dae::GameObject* pOwner);
		
		void Update() override;
		void Initialize() override;
		void Render() const override;
	private:
		void StartGame(QBertGameContext::GameMode gameMode) const;
	};

}
