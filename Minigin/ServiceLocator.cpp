#include "MiniginPCH.h"
#include "ServiceLocator.h"

dae::NullSoundSystem dae::ServiceLocator::m_NullSoundSystem{};
dae::SoundSystem* dae::ServiceLocator::m_pSoundSystem {&m_NullSoundSystem};

dae::SoundSystem& dae::ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSystem;
}

void dae::ServiceLocator::RegisterSoundSystem(SoundSystem* pSoundSystem)
{
	m_pSoundSystem = pSoundSystem == nullptr ? &m_NullSoundSystem : pSoundSystem;
}
