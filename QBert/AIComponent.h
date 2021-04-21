#pragma once
#include <Component.h>

namespace QBert
{
	class MovementComponent;
	class MoveCommand;

	class AIComponent : public dae::Component
	{
	public:
		enum class EnemyType
		{
			Coily,
			Ugg,
			WrongWay,
			Slick,
			Sam
		};
		
		AIComponent(dae::GameObject* pOwner, EnemyType type, MovementComponent* pMovementComponent, dae::GameObject* pPlayer = nullptr);
		~AIComponent() override;
		void Update() override;

		EnemyType GetType() const;
	private:
		EnemyType m_Type;
		dae::GameObject* m_pPlayer;

		const float m_movementInterval{ 2.f };
		float m_movementTime;
		
		MoveCommand* m_pUpCommand;
		MoveCommand* m_pDownCommand;
		MoveCommand* m_pLeftCommand;
		MoveCommand* m_pRightCommand;
	};

}
