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
			Reverting,
			Disc
		};
		
		PlayableTerrainComponent(dae::GameObject* pOwner, TerrainType type, dae::TextureComponent* pPlatform);
		~PlayableTerrainComponent() override = default;

		void Initialize() override;
		void Update() override;

		void Activate();
		void Revert();

		TerrainType GetType() const;
		
		void AddObserver(dae::Observer* pObserver);
	private:
		dae::TextureComponent* m_pPlatform;

		glm::vec3 m_ColorInactive{ 200,200,200 };
		glm::vec3 m_ColorIntermediate{ 255,255,0 };
		glm::vec3 m_ColorActive{ 0,255,0 };

		TerrainType m_Type;
		int m_StepsNeeded;

		dae::Subject m_Subject;

		auto SetTextureComponentColors() const -> void;
	};

}
