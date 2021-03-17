#pragma once
#include <future>
#include <mutex>
#include <queue>

namespace dae
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		void QueueSound(int soundId, float volume = 1.f);
		void Start();
		void Stop();
		
		SoundSystem() = default;
		SoundSystem(const SoundSystem&) = delete;
		SoundSystem& operator=(const SoundSystem&) = delete;
		SoundSystem(SoundSystem&&) = delete;
		SoundSystem& operator=(SoundSystem&&) = delete;

	private:
		virtual void Play(int soundId, float volume = 1.f) = 0;
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

	class NullSoundSystem final : public SoundSystem
	{
	public:
		~NullSoundSystem() override = default;
	private:
		void Play(int , float ) override {};
	};
}