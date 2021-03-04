#pragma once


namespace dae
{
	class Event;

	class Observer
	{
	public:

		virtual void OnNotify(const Event& event) = 0;

		Observer() = default;
		virtual ~Observer() = default;

		Observer(const Observer&) = delete;
		Observer& operator=(const Observer&) = delete;
		Observer(Observer&&) = delete;
		Observer& operator=(Observer&&) = delete;
	};


}
