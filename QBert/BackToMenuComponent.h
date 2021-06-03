#pragma once
#include "RenderComponent.h"

namespace QBert
{
	class BackToMenuComponent : public dae::RenderComponent
	{
	public:
		BackToMenuComponent(dae::GameObject* pOwner);
		
		void Initialize() override;
		void Render() const override;
	private:
		void GoToMainMenu() const;
	};
}

