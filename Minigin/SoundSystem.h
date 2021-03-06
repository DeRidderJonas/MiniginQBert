#pragma once
#include <string>

namespace dae
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const std::string& filePath, float volume = 1.f) = 0;
		virtual void ToggleMute() = 0;
		
		SoundSystem() = default;
		SoundSystem(const SoundSystem&) = delete;
		SoundSystem& operator=(const SoundSystem&) = delete;
		SoundSystem(SoundSystem&&) = delete;
		SoundSystem& operator=(SoundSystem&&) = delete;
		
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		~NullSoundSystem() override = default;
		void Play(const std::string& , float ) override {};
		void ToggleMute() override{};
	};
}