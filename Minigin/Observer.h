#pragma once

namespace dae
{
	class Component;
	class GameObject;

	class Observer
	{
	public:
		struct Event
		{
			std::string name;
			double value;
			GameObject* pOwner;
			Component* pInstigator;
		};

		virtual void OnNotify(const Event& event) = 0;

		Observer() = default;
		virtual ~Observer() = default;

		Observer(const Observer&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(Observer&&) = delete;
	};


}