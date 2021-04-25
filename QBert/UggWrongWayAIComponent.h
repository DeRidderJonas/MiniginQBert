#pragma once
#include "AIComponent.h"

namespace QBert
{
	class UggWrongWayAIComponent : public AIComponent
	{
	public:
		UggWrongWayAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer);
		virtual ~UggWrongWayAIComponent() override = default;
		
		void OnCollisionWithPlayer(dae::GameObject* pPlayer) override;
		void OnReachBottom() override;
	protected:
		MovementComponent::Direction GetNextDirectionToGo() const override;
	};


}