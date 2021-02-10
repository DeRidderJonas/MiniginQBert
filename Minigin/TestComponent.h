#pragma once
#include "Component.h"
namespace dae
{
	class TestComponent :
		public Component
	{
	public:
		TestComponent(const GameObject* pOwner) : Component(pOwner) {};
	};
}

