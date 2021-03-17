#include "MiniginPCH.h"
#include "GameTime.h"


void dae::GameTime::Start()
{
	m_IsRunning = true;
	m_LastTime = std::chrono::high_resolution_clock::now();
}

void dae::GameTime::Update()
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

int dae::GameTime::GetFPS() const
{
	return m_FPS;
}

float dae::GameTime::GetDeltaTime() const
{
	return m_DeltaTime;
}

const std::chrono::high_resolution_clock::time_point& dae::GameTime::GetNow() const
{
	return m_LastTime;
}

dae::GameTime::GameTime()
	: m_IsRunning{false}
	, m_LastTime{}
	, m_DeltaTime{0.f}
	, m_FPS{0}
	, m_FPSTimer{0.f}
	, m_FPSCounter{0}
{
}
