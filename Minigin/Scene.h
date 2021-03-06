#pragma once
#include "RenderComponent.h"
#include "SceneManager.h"


namespace dae
{
	class GameContext;
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(GameObject* pObject);

		void SortRenderComponents();

		void Update();
		void Render() const;

		void DestroyAll();
		
		const std::string& GetName() const;
		void SetGameContext(GameContext* pGameContext, bool deletePrevious = true);
		GameContext* GetGameContext() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector <GameObject*> m_Objects{};
		std::vector<GameObject*> m_ObjectsToAdd{};
		std::vector<RenderComponent*> m_RenderComponents{};
		GameContext* m_pGameContext;

		static unsigned int m_IdCounter; 
	};

}
