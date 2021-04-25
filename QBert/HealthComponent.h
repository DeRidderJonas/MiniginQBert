#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae {
	class Observer;
}

namespace QBert
{
	class HealthComponent : public dae::Component
	{
	public:
		enum class HealthOwner
		{
			QBert,
			Coily,
			SlickSam,
			UggWrongWay
		};
		HealthComponent(dae::GameObject* pOwner, HealthOwner healthOwner, int amountOfLives);
		~HealthComponent() override = default;

		void Initialize() override;
		void Update() override;

		void Kill(bool awardPoints = true);
		const HealthOwner& GetHealthOwnerType() const;

		void AddObserver(dae::Observer* pObserver);
	private:
		HealthOwner m_HealthOwner;
		dae::Subject m_pSubject;

		int m_AmountOfLives;
	};


}
