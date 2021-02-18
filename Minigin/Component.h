#pragma once
#include "GameObject.h"

namespace dae
{
	class Component
	{
	public:
		Component(const GameObject* pOwner) : m_pOwner{ pOwner } {};
		virtual ~Component() = default;
		virtual void Update() {};

		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;
	protected:
		const GameObject* m_pOwner;
	};

}
