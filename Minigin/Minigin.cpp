#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#pragma warning(push)
#pragma warning(disable:26812)
#include <SDL.h>
#pragma warning(pop)

#include "audio.h"

#include "ConsoleSoundSystem.h"
#include "GameObject.h"
#include "Scene.h"
#include "GameTime.h"
#include "TransformComponent.h"
#include "FPSComponent.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "GameModeMenuComponent.h"
#include "HealthComponent.h"
#include "KillCommand.h"
#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "ServiceLocator.h"
#include "SimpleSDL2AudioSoundSystem.h"
#include "SoundCommand.h"
#include "TextureLineComponent.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
	_putenv("SDL_AUDIODRIVER=DirectSound");
	SDL_Init(SDL_INIT_AUDIO);
	initAudio();
}

void dae::Minigin::Cleanup()
{
	endAudio();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(std::function<void()> loadGame)
{
	Initialize();
	auto pSoundSystem = new SimpleSDL2AudioSoundSystem();
	ServiceLocator::RegisterSoundSystem(pSoundSystem);

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	loadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& time = GameTime::GetInstance();

		ServiceLocator::GetSoundSystem().Start();

		std::cout << std::this_thread::get_id() << '\n';
		
		bool doContinue = true;
		
		time.Start();
		while (doContinue)
		{
			time.Update();

			doContinue = input.ProcessInput();

			sceneManager.Update(); //Uses Time.DeltaTime
			
			renderer.Render();

			auto sleepTime = duration_cast<duration<float>>(time.GetNow() + milliseconds(MsPerFrame) - high_resolution_clock::now());
			if (sleepTime.count() > 0.f) this_thread::sleep_for(sleepTime);
		}
	}

	ServiceLocator::GetSoundSystem().Stop();
	delete pSoundSystem;
	Cleanup();
}
