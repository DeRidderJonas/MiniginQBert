#pragma once
#include "Component.h"
#include "Observer.h"

namespace QBert
{
	class TextureLineComponent;

	class LivesDisplayComponent final : public dae::Component, public dae::Observer
	{
	public:
		LivesDisplayComponent(dae::GameObject* pOwner, TextureLineComponent* pTextureLineComponent, int amountOfLives);
		virtual ~LivesDisplayComponent() = default;

		void Update() override;

		void OnNotify(const dae::Event& event) override;
	private:
		TextureLineComponent* m_pTextureLineComponent{};
	};

}
