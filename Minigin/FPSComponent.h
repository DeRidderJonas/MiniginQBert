#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace dae
{
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* pOwner, TextComponent* pTextComponent);
		virtual ~FPSComponent() = default;

		virtual void Update() override;
	private:
		TextComponent* m_pTextComponent;
	};
}

