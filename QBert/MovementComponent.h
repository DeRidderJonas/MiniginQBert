#pragma once
#include "Component.h"

namespace QBert
{
	class MovementComponent final : public dae::Component
	{
	public:
		enum class Direction
		{
			UP,
			DOWN,
			LEFT,
			RIGHT
		};
		
		MovementComponent(dae::GameObject* pOwner, dae::GameObject* pStandOn);
		~MovementComponent() override = default;
		void Update() override;
		void Move(Direction direction, bool activatesTerrain = false);
	private:
		dae::GameObject* m_pStandingOn;
	};
}

