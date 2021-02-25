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
			SlickSam
		};
		HealthComponent(dae::GameObject* pOwner, HealthOwner healthOwner);
		~HealthComponent() override = default;
		
		void Update() override;

		void Kill();
		const HealthOwner& GetHealthOwnerType() const;

		void AddObserver(dae::Observer* pObserver);
	private:
		HealthOwner m_HealthOwner;
		dae::Subject m_pSubject;
	};


}
