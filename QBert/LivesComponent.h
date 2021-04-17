#pragma once
#include "Component.h"
#include "Observer.h"

namespace QBert
{
	class TextureLineComponent;

	class LivesComponent final : public dae::Component, public dae::Observer
	{
	public:
		LivesComponent(dae::GameObject* pOwner, TextureLineComponent* pTextureLineComponent, int amountOfLives);
		virtual ~LivesComponent() = default;

		void Update() override;

		void OnNotify(const dae::Event& event) override;
	private:
		int m_AmountOfLives{};
		
		TextureLineComponent* m_pTextureLineComponent{};
	};

}
