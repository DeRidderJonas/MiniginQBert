#include "MiniginPCH.h"
#include "Time.h"


void dae::Time::Start()
{
	m_IsRunning = true;
	m_LastTime = std::chrono::high_resolution_clock::now();
}

void dae::Time::Update()
{
	if (!m_IsRunning)
	{
		m_FPS = 0;
		m_DeltaTime = 0.f;

		return;
	}

	auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();
	m_LastTime = currentTime;

	//Prefer this method to only update FPS once every second (Don't spasm the counter)
	m_FPSTimer += m_DeltaTime;
	++m_FPSCounter;
	if (m_FPSTimer >= 1.f)
	{
		m_FPS = m_FPSCounter;
		m_FPSCounter = 0;
		m_FPSTimer -= 1.f;
	}
}

int dae::Time::GetFPS()
{
	return m_FPS;
}

float dae::Time::GetDeltaTime()
{
	return m_DeltaTime;
}

const std::chrono::high_resolution_clock::time_point& dae::Time::GetNow() const
{
	return m_LastTime;
}

dae::Time::Time()
	: m_IsRunning{false}
	, m_LastTime{}
	, m_DeltaTime{0.f}
	, m_FPS{0}
	, m_FPSTimer{0.f}
	, m_FPSCounter{0}
{
}
