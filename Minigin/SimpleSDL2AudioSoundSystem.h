#pragma once
#include "SoundSystem.h"
#include <mutex>
#include <queue>

namespace dae
{
	class SimpleSDL2AudioSoundSystem final : public SoundSystem
	{
	public:
		SimpleSDL2AudioSoundSystem();
		~SimpleSDL2AudioSoundSystem() override;
		void Play(int soundId, float volume) override;
		void ToggleMute() override;
	private:
		void Update();

		struct PlaySound
		{
			int soundId;
			float volume;
		};

		bool m_Muted{ false };
		
		std::atomic_bool m_playing;
		std::mutex m_mutex;
		std::condition_variable m_QueueActive{};
		std::thread m_soundThread;
		std::queue<PlaySound> m_SoundQueue{};
	};
}