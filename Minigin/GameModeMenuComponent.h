#pragma once
#include "RenderComponent.h"

namespace dae
{
	class GameModeMenuComponent : public RenderComponent
	{
	public:
		GameModeMenuComponent(GameObject* pOwner);
		
		void Update() override;
		void Render() const override;
	};

}
