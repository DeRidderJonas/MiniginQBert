#pragma once
#include "Component.h"

namespace QBert
{
	class MovementComponent final : public dae::Component
	{
	public:
		enum class Direction : int
		{
			UP = 0,
			DOWN = 1,
			LEFT = 2,
			RIGHT = 3
		};
		
		MovementComponent(dae::GameObject* pOwner, dae::GameObject* pStandOn = nullptr);
		~MovementComponent() override = default;
		void Update() override;
		void Move(Direction direction, bool activatesTerrain = false, bool revertsTerrain = false);
		void GoToSpawningPlatform();
	private:
		dae::GameObject* m_pStandingOn;
	};
}

