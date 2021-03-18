#include "MiniginPCH.h"
#include "SimpleSDL2AudioSoundSystem.h"
#include "audio.h"

dae::SimpleSDL2AudioSoundSystem::SimpleSDL2AudioSoundSystem()
{
	m_playing.store(true);
	auto self = this;
	m_soundThread = std::thread([self]() {self->Update(); });
}

dae::SimpleSDL2AudioSoundSystem::~SimpleSDL2AudioSoundSystem()
{
	m_playing.store(false);
	m_QueueActive.notify_one();
	m_soundThread.join();
}

void dae::SimpleSDL2AudioSoundSystem::Play(int soundId, float volume)
{
	PlaySound playSound{ soundId, volume };
	std::lock_guard<std::mutex> mlock{ m_mutex };
	m_SoundQueue.push(playSound);
	m_QueueActive.notify_one();
}

void dae::SimpleSDL2AudioSoundSystem::Update()
{
	while (m_playing.load())
	{
		while (!m_SoundQueue.empty())
		{
			PlaySound soundToPlay = m_SoundQueue.front();
			m_SoundQueue.pop();
			std::unique_lock<std::mutex> mlock{ m_mutex };
			playSound("../Data/door1.wav", int(SDL_MIX_MAXVOLUME * soundToPlay.volume));
		}

		std::unique_lock<std::mutex> guard{ m_mutex };
		m_QueueActive.wait(guard);
	}
}
