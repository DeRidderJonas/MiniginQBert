#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "Component.h"

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* pOwner);
		virtual ~TransformComponent() = default;

		void Update() override{};
		
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& pos);
		void SetPosition(float x, float y, float z);
		void SetPosition(float x, float y);
	private:
		glm::vec3 m_Position;
	};
}

