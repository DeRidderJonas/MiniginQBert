#pragma once
#include "Singleton.h"

namespace dae {
	class Scene;
	class GameObject;
}

namespace QBert
{
	class LevelManager final : public dae::Singleton<LevelManager>
	{
	public:
		void CreateLevel(dae::Scene& pScene);
		dae::GameObject* GetGameObject(int row, int col) const;
		void GetPositionForGameObject(dae::GameObject* pToFind, int& row, int& col);
	private:
		friend class Singleton<LevelManager>;
		LevelManager();

		const static int m_width{ 7 };
		dae::GameObject* m_GameObjects[m_width][m_width]{};
	};

}
