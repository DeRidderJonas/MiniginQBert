#pragma once
#include "Event.h"

namespace dae {
	class GameObject;
}

namespace QBert
{
	class MoveEvent final : public dae::Event
	{
	public:
		MoveEvent(const std::string& name, dae::GameObject* pMover);
		~MoveEvent() override = default;

		dae::GameObject* GetMover() const;
	private:
		dae::GameObject* m_pMover;
	};
}
