#pragma once
#include <Component.h>

#include "MovementComponent.h"

namespace QBert
{
	class MovementComponent;
	class MoveCommand;

	class AIComponent : public dae::Component
	{
	public:
		enum class EnemyType
		{
			Coily = 0,
			UggWrongWay = 1,
			SlickSam = 2
		};
		
		AIComponent(dae::GameObject* pOwner, EnemyType type, dae::GameObject* pPlayer = nullptr);
		~AIComponent() override;

		void Initialize() override;
		void Update() override;

		EnemyType GetType() const;
		virtual void OnReachBottom() = 0;
		virtual void OnCollisionWithPlayer(dae::GameObject* pPlayer) = 0;
	protected:
		virtual MovementComponent::Direction GetNextDirectionToGo() const = 0;
		
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
