#pragma once
#include "Component.h"

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* pOwner, int renderPriority = 0);
		virtual ~RenderComponent() = default;

		void Update() override;
		virtual void Render() const = 0;

		void SetRenderPriority(int value);
		int GetRenderPriority() const;
	private:
		//High -> Last to draw
		int m_RenderPriority;
	};

}
