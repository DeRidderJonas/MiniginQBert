#pragma once
#include "AIComponent.h"

namespace QBert
{
	class SlickSamAIComponent : public AIComponent
	{
	public:
		SlickSamAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer);
		virtual ~SlickSamAIComponent() override = default;

		void OnReachBottom() override;
	protected:
		MovementComponent::Direction GetNextDirectionToGo() const override;
	};

}
