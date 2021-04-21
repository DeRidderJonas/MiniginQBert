#pragma once
#include "Component.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "Subject.h"

namespace dae {
	class TextureComponent;
}

namespace QBert
{
	class ScoreComponent;

	class PlayableTerrainComponent : public dae::Component
	{
	public:
		enum class TerrainType
		{
			Normal,
			Double,
			Reverting
		};
		
		PlayableTerrainComponent(dae::GameObject* pOwner, TerrainType type, dae::TextureComponent* pTop, dae::TextureComponent* pLeft, dae::TextureComponent* pRight);
		~PlayableTerrainComponent() override = default;
		void Update() override;
		void Activate();
		void Revert();

		void AddObserver(dae::Observer* pObserver);
	private:
		dae::TextureComponent* m_pTop;
		dae::TextureComponent* m_pLeft;
		dae::TextureComponent* m_pRight;

		glm::vec3 m_ColorInactive{ 200,200,200 };
		glm::vec3 m_ColorIntermediate{ 255,255,0 };
		glm::vec3 m_ColorActive{ 0,255,0 };

		TerrainType m_Type;
		int m_StepsNeeded;

		dae::Subject m_Subject;

		auto SetTextureComponentColors() const -> void;
	};

}
