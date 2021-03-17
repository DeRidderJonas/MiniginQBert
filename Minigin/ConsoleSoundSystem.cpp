#include "MiniginPCH.h"
#include "ConsoleSoundSystem.h"

void dae::ConsoleSoundSystem::Play(int soundId, float volume)
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << std::this_thread::get_id() << ": Playing sound with id: " << soundId << " at volume: " << volume << '\n';
}
