#pragma once
#include "SoundSystem.h"

namespace dae
{
	class SimpleSDL2AudioSoundSystem final : public SoundSystem
	{
	public:
		SimpleSDL2AudioSoundSystem();
		~SimpleSDL2AudioSoundSystem() override;
		void Play(int soundId, float volume) override;
	private:
		void Update();

		struct PlaySound
		{
			int soundId;
			float volume;
		};

		std::atomic_bool m_playing;
		std::mutex m_mutex;
		std::thread m_soundThread;
		std::queue<PlaySound> m_SoundQueue{};
	};
}