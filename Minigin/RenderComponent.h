#pragma once
#include "Component.h"

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(const std::shared_ptr<GameObject>& pOwner) : Component(pOwner) {};
		virtual ~RenderComponent() = default;

		virtual void Render() const = 0;
	};

}
