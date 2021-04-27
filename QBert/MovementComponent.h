#pragma once
#include "Component.h"
#include "Subject.h"


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

		void Initialize() override;
		void Update() override;

		void Move(Direction direction, bool activatesTerrain = false, bool revertsTerrain = false, bool canStandOnDisc = false);
		void GoToSpawningPlatform();
		dae::GameObject* GetPlatform() const;

		void AddObserver(dae::Observer* pObserver);
	private:
		void FallOffGrid();
		
		dae::GameObject* m_pStandingOn;
		dae::Subject m_Subject;
	};
}

