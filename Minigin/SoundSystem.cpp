#include "MiniginPCH.h"
#include "SoundSystem.h"

void dae::SoundSystem::QueueSound(int soundId, float volume)
{
	PlaySound playSound{ soundId, volume };
	auto self = this;
	std::thread addThread([&playSound, self]()
		{
			std::unique_lock<std::mutex> mlock{ self->m_mutex };
			std::cout << std::this_thread::get_id() << " queues" << '\n';
			self->m_SoundQueue.push(playSound);
		});
	addThread.detach(); //TODO: fix crash if closed before thread finishes (mutex locked)
}

void dae::SoundSystem::Start()
{
	m_playing.store(true);
	auto self = this;
	m_soundThread = std::thread([self]() {self->Update(); });
}

void dae::SoundSystem::Stop()
{
	m_playing.store(false);
	m_soundThread.join();
}

void dae::SoundSystem::Update()
{
	while(m_playing.load())
	{
		std::unique_lock<std::mutex> mlock{ m_mutex };
		while (!m_SoundQueue.empty())
		{
			PlaySound& playSound = m_SoundQueue.front();
			std::cout << std::this_thread::get_id() << " consumes" << '\n';
			Play(playSound.soundId, playSound.volume);
			std::cout << std::this_thread::get_id() << " consumed" << '\n';
			m_SoundQueue.pop();
		}
	}
}
