#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Time final : public Singleton<Time>
	{
	public:
		void Start();
		void Update();
		int GetFPS();
		float GetDeltaTime();

		const std::chrono::high_resolution_clock::time_point& GetNow() const;
		void SetFixedUpdateDeltaTime(float value);
		float GetFixedUpdateDeltaTime();
	private:
		friend class Singleton<Time>;
		Time();

		bool m_IsRunning = false;
		std::chrono::high_resolution_clock::time_point m_LastTime;
		float m_DeltaTime;
		float m_FixedUpdateDeltaTime;
		int m_FPS;
		float m_FPSTimer;
		int m_FPSCounter;
	};
}

