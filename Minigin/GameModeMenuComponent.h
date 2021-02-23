#pragma once
#include "RenderComponent.h"

namespace QBert
{
	class GameModeMenuComponent : public dae::RenderComponent
	{
	public:
		GameModeMenuComponent(dae::GameObject* pOwner);
		
		void Update() override;
		void Render() const override;
	};

}
