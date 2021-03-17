#pragma once
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(SoundSystem* pSoundSystem);
	private:
		static SoundSystem* m_pSoundSystem;
		static NullSoundSystem m_NullSoundSystem;
	};
}
