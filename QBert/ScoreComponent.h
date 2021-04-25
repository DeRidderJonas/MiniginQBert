#pragma once
#include "Component.h"
#include "Event.h"
#include "Observer.h"

namespace dae {
	class TextComponent;
}

namespace QBert
{	
	class ScoreComponent : public dae::Component, public dae::Observer
	{
	public:
		ScoreComponent(dae::GameObject* pOwner, dae::TextComponent* pTextComponent);
		~ScoreComponent() override = default;

		void Initialize() override;
		void Update() override;

		void OnNotify(const dae::Event& event) override;
	private:
		dae::TextComponent* m_pTextComponent;
		int m_Score;
	};


}
