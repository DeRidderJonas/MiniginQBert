#pragma once

namespace dae
{
	class Component;
	class GameObject;

	class Command
	{
	public:
		Command() = default;
		Command(GameObject* pDestination);
		Command(Component* pDestination);
		virtual ~Command() = default;
		virtual void Execute() = 0;
	protected:
		GameObject* m_pGameObject;
		Component* m_pComponent;
	};

}
