#pragma once
namespace dae {
	class Scene;
	class GameObject;

	class GameContext
	{
	public:
		GameContext(Scene* pScene);
		virtual ~GameContext() = default;

		virtual void OnAddGameObject(dae::GameObject* pGameObject) = 0;
		virtual void OnRemoveGameObject(dae::GameObject* pGameObject) = 0;

		virtual void Update() = 0;
	protected:
		Scene* m_pScene;
	};
}

