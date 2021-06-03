#pragma once
#include "AIComponent.h"

namespace QBert
{
	class CoilyAIComponent : public AIComponent
	{
	public:
		CoilyAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer = nullptr);
		virtual ~CoilyAIComponent() override = default;
		void Update() override;
		
		void OnReachBottom() override;
		void OnCollisionWithPlayer(dae::GameObject* pPlayer) override;

		void SetTarget(dae::GameObject* pTarget);
	protected:
		MovementComponent::Direction GetNextDirectionToGo() const override;
	private:
		const float m_UpdateTargetInterval{ m_movementInterval * 2.f + 0.1f };
		float m_updateTargetRemaining;
		int m_TargetRow;
		float m_TargetX;
		
		bool m_HasHatched;
	};

}
