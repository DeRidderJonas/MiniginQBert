#pragma once
#include "Transform.h"
#include <memory>

namespace dae
{
	class Component;
	class GameObject final
	{
	public:
		void Update();

		void SetPosition(float x, float y);

		const Transform& GetTransform() const;
		template<typename ComponentType>
		std::shared_ptr<ComponentType> GetComponentOfType()
		{
			auto componentIt = std::find_if(m_Components.begin(), m_Components.end(), [](const std::shared_ptr<Component>& component) {
				auto convertedPtr = std::dynamic_pointer_cast<ComponentType>(component);
				return convertedPtr != nullptr;
				//return typeid(*component) == typeid(ComponentType);
				//return std::is_base_of<ComponentType, typeid(component)>::value;
			});
			if (componentIt == m_Components.end()) return nullptr;
			
			return std::static_pointer_cast<ComponentType>(*componentIt);
		}
		void AddComponent(const std::shared_ptr<Component>& pComponent);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	private:
		Transform m_Transform;
		std::vector<std::shared_ptr<Component>> m_Components;
	};
}
