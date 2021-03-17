#pragma once
#include "SoundSystem.h"

namespace dae
{
	class SimpleSDL2AudioSoundSystem final : public SoundSystem
	{
	public:
		~SimpleSDL2AudioSoundSystem() override = default;
	private:
		void Play(int soundId, float volume) override;
	};
}