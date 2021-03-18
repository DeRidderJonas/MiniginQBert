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
	m_soundThread.join();
}

void dae::SimpleSDL2AudioSoundSystem::Play(int soundId, float volume)
{
	PlaySound playSound{ soundId, volume };
	std::unique_lock<std::mutex> mlock{ m_mutex };
	m_SoundQueue.push(playSound);
}

void dae::SimpleSDL2AudioSoundSystem::Update()
{
	while (m_playing.load())
	{
		std::unique_lock<std::mutex> mlock{ m_mutex };
		while (!m_SoundQueue.empty())
		{
			PlaySound& soundToPlay = m_SoundQueue.front();
			playSound("../Data/door1.wav", int(SDL_MIX_MAXVOLUME * soundToPlay.volume));
			m_SoundQueue.pop();
		}
	}
}
