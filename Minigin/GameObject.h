#pragma once
#include <memory>
#include <vector>

namespace dae
{
	class Scene;
	class Component;
	class GameObject final
	{
	public:
		void Initialize();
		void Update();

		template<typename ComponentType>
		ComponentType* GetComponentOfType() const
		{
			auto componentIt = std::find_if(m_Components.begin(), m_Components.end(), [](const Component* component) {
				auto convertedPtr = dynamic_cast<const ComponentType*>(component);
				return convertedPtr != nullptr;
			});
			if (componentIt == m_Components.end()) return nullptr;
			
			return static_cast<ComponentType*>(*componentIt);
		}
		template<typename ComponentType>
		std::vector<ComponentType*> GetAllComponentsOfType() const
		{
			std::vector<ComponentType*> components{};
			for(Component* pComponent : m_Components)
			{
				ComponentType* pConverted = dynamic_cast<ComponentType*>(pComponent);
				if (pConverted) components.push_back(pConverted);
			}
			return components;
		}
		void AddComponent(Component* pComponent);

		void Destroy();
		bool ShouldBeDestroyed() const;

		void SetScene(Scene* pScene);
		Scene* GetScene() const;
		
		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	private:
		std::vector<Component*> m_Components;
		bool m_ToBeDestroyed;
		Scene* m_pScene;
	};
}
