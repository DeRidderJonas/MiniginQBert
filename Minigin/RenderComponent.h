#pragma once
#include "Component.h"

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(const GameObject* pOwner);
		virtual ~RenderComponent() = default;

		void Update() override;
		virtual void Render() const = 0;
	};

}
