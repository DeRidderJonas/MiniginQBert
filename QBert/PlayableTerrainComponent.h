#pragma once
#include "Component.h"

namespace dae {
	class TextureComponent;
}

namespace QBert
{
	class PlayableTerrainComponent : public dae::Component
	{
	public:
		PlayableTerrainComponent(dae::GameObject* pOwner, dae::TextureComponent* pTop, dae::TextureComponent* pLeft, dae::TextureComponent* pRight);
		~PlayableTerrainComponent() override = default;
		void Update() override;
	private:
		dae::TextureComponent* m_pTop;
		dae::TextureComponent* m_pLeft;
		dae::TextureComponent* m_pRight;
	};

}
