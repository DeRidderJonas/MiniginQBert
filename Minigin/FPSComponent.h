#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include "TextComponent.h"

namespace dae
{
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* pOwner, TextComponent* pRenderComponent);
		virtual ~FPSComponent() = default;

		virtual void Update() override;
		void Initialize() override;
	private:
		TextComponent* m_pTextComponent;
		int m_FPS;
	};
}

