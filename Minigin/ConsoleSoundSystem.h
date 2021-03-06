#pragma once
#include "SoundSystem.h"

namespace dae
{
	class ConsoleSoundSystem final : public SoundSystem
	{
	public:
		ConsoleSoundSystem(SoundSystem* pSoundSystem);
		~ConsoleSoundSystem() override = default;
		void Play(const std::string& filePath, float volume) override;
		void ToggleMute() override;
	private:
		SoundSystem* m_pSoundSystem{nullptr};
		bool m_IsMuted{ false };
	};

}
