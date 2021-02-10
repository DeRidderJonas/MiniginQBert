#pragma once
#include "GameObject.h"

namespace dae
{
	class Component
	{
	public:
		Component(const std::shared_ptr<GameObject>& pOwner) : m_pOwner{ pOwner } {};
		virtual ~Component() = default;

		Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(Component&&) = delete;
	protected:
		std::weak_ptr<GameObject> m_pOwner;
	};

}
