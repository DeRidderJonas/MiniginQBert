#pragma once
#include "SoundSystem.h"

namespace dae
{
	class ConsoleSoundSystem final : public SoundSystem
	{
	public:
		ConsoleSoundSystem(SoundSystem* pSoundSystem, bool isMuted);
		~ConsoleSoundSystem() override = default;
		void Play(int soundId, float volume) override;
	private:
		SoundSystem* m_pSoundSystem{nullptr};
		bool m_IsMuted{ false };
	};

}
