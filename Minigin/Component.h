#pragma once
#include "GameObject.h"

namespace dae
{
	class Component
	{
	public:
		Component(GameObject* pOwner) : m_pOwner{ pOwner } {};
		virtual ~Component() = default;
		virtual void Update() = 0;
		GameObject* GetOwner() const { return m_pOwner; }

		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;
	protected:
		GameObject* m_pOwner;
	};

}
