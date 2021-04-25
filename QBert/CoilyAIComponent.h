#pragma once
#include "AIComponent.h"

namespace QBert
{
	class CoilyAIComponent : public AIComponent
	{
	public:
		CoilyAIComponent(dae::GameObject* pOwner, dae::GameObject* pPlayer = nullptr);
		virtual ~CoilyAIComponent() override = default;
		
		void OnReachBottom() override;
	protected:
		MovementComponent::Direction GetNextDirectionToGo() const override;
	private:
		bool m_HasHatched;
	};

}
