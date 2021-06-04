#pragma once
#include "Component.h"
#include "Subject.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

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
		void GoToSpawningPlatform(bool isPlayer = false, bool isPlayerTwo = false);
		dae::GameObject* GetPlatform() const;

		void AddObserver(dae::Observer* pObserver);
	private:
		void FallOffGrid();

		const float m_moveTime{ 0.3f };
		float m_movedTime;
		glm::vec2 m_moveFrom;
		
		dae::GameObject* m_pStandingOn;
		dae::Subject m_Subject;
	};
}

