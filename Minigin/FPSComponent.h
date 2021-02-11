#pragma once
#include "Component.h"
#include "RenderComponent.h"

namespace dae
{
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* pOwner, RenderComponent* pRenderComponent);
		virtual ~FPSComponent() = default;

		virtual void Update() override;
	private:
		RenderComponent* m_pRenderComponent;
	};
}

