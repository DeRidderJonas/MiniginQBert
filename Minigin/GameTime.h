#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		void Start();
		void Update();
		int GetFPS() const;
		float GetDeltaTime() const;

		const std::chrono::high_resolution_clock::time_point& GetNow() const;
	private:
		friend class Singleton<GameTime>;
		GameTime();

		bool m_IsRunning = false;
		std::chrono::high_resolution_clock::time_point m_LastTime;
		float m_DeltaTime;
		int m_FPS;
		float m_FPSTimer;
		int m_FPSCounter;
	};
}

