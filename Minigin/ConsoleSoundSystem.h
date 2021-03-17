#pragma once
#include "SoundSystem.h"

namespace dae
{
	class ConsoleSoundSystem final : public SoundSystem
	{
	public:
		~ConsoleSoundSystem() override = default;
	private:
		void Play(int soundId, float volume) override;
	};

}
