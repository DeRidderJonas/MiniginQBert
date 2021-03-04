#pragma once

namespace dae
{
	class Event
	{
	public:
		Event(const std::string& name);
		virtual ~Event() = default;

		const std::string& GetName() const;
	private:
		std::string m_Name;
	};

}
