#pragma once
#include "AIComponent.h"

namespace QBert
{
	class UggWrongWayAIComponent final : public AIComponent
	{
	public:
		UggWrongWayAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer, bool goLeft);
		virtual ~UggWrongWayAIComponent() override = default;
		
		void OnCollisionWithPlayer(dae::GameObject* pPlayer) override;
		void OnReachBottom() override;
	protected:
		MovementComponent::Direction GetNextDirectionToGo() const override;
	private:
		bool m_GoingLeft;
	};


}