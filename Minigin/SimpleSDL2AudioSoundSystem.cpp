#include "MiniginPCH.h"
#include "SimpleSDL2AudioSoundSystem.h"
#include "audio.h"

void dae::SimpleSDL2AudioSoundSystem::Play(int , float )
{
	playMusic("../Data/door1.wav", SDL_MIX_MAXVOLUME);
	SDL_Delay(2000);
}
