#pragma once
#include "Component.h"
namespace dae
{
	class TestComponent :
		public Component
	{
	public:
		TestComponent(const std::shared_ptr<GameObject>& pOwner) : Component(pOwner) {};
	};
}

