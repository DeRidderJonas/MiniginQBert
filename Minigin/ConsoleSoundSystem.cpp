#include "MiniginPCH.h"
#include "ConsoleSoundSystem.h"

dae::ConsoleSoundSystem::ConsoleSoundSystem(SoundSystem* pSoundSystem)
	: m_pSoundSystem{pSoundSystem}
{
}

void dae::ConsoleSoundSystem::Play(int soundId, float volume)
{
	if (!m_IsMuted)
	{
		std::cout << "Playing sound with id: " << soundId << " at volume: " << volume << '\n';
		if(m_pSoundSystem) m_pSoundSystem->Play(soundId, volume);
	}
}

void dae::ConsoleSoundSystem::ToggleMute()
{
	m_IsMuted = !m_IsMuted;
	if (m_pSoundSystem) m_pSoundSystem->ToggleMute();
}
