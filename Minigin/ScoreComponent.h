#pragma once
#include "Component.h"
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
		void Update() override;
		void OnNotify(const dae::Observer::Event& event) override;
	private:
		dae::TextComponent* m_pTextComponent;
		int m_Score;
	};


}
