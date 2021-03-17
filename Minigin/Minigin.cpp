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
#include "Time.h"
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

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto& input = InputManager::GetInstance();

	auto go = new GameObject();
	auto renderComponent = new TextureComponent(go, "logo.png");
	go->AddComponent(renderComponent);
	go->GetComponentOfType<TextureComponent>()->SetTexture("background.jpg");
	scene.Add(go);

	go = new GameObject();
	renderComponent = new TextureComponent(go, "logo.png");
	auto transform = go->GetComponentOfType<TransformComponent>();
	transform->SetPosition(216, 180);

	go->AddComponent(renderComponent);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = new GameObject();
	renderComponent = new TextureComponent(go);
	auto textComponent = new TextComponent(go, renderComponent, "Programming 4 Assignment", font);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	transform = go->GetComponentOfType<TransformComponent>();
	transform->SetPosition(80, 20);
	scene.Add(go);

	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	go = new GameObject();
	renderComponent = new TextureComponent(go);
	textComponent = new TextComponent(go, renderComponent, "FPS", font);
	auto fpsComponent = new FPSComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(fpsComponent);
	transform = go->GetComponentOfType<TransformComponent>();
	transform->SetPosition(5, 5);
	scene.Add(go);

	go = new GameObject();
	QBert::GameModeMenuComponent* gmmc = new QBert::GameModeMenuComponent(go);
	go->AddComponent(gmmc);
	scene.Add(go);

	go = new GameObject();
	QBert::TextureLineComponent* tlc = new QBert::TextureLineComponent(go, "Life.png");
	QBert::LivesComponent* lc = new QBert::LivesComponent(go, tlc, 5);
	go->AddComponent(tlc);
	go->AddComponent(lc);
	go->GetComponentOfType<TransformComponent>()->SetPosition(5.f, 50.f);
	scene.Add(go);

	go = new GameObject();
	renderComponent = new TextureComponent(go);
	textComponent = new TextComponent(go, renderComponent, "", font);
	auto scoreComponent = new QBert::ScoreComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(scoreComponent);
	go->GetComponentOfType<TransformComponent>()->SetPosition(5.f, 100.f);
	scene.Add(go);
	
	GameObject* QBert = new GameObject();
	QBert::HealthComponent* hc = new QBert::HealthComponent(QBert, QBert::HealthComponent::HealthOwner::QBert);
	hc->AddObserver(lc);
	QBert->AddComponent(hc);
	scene.Add(QBert);

	input.Bind(0,ControllerButton::ButtonA, std::make_shared<QBert::KillCommand>(hc), InputState::pressed);

	GameObject* coily = new GameObject();
	hc = new QBert::HealthComponent(coily, QBert::HealthComponent::HealthOwner::Coily);
	hc->AddObserver(scoreComponent);
	coily->AddComponent(hc);
	scene.Add(coily);

	auto killCommand = std::make_shared<QBert::KillCommand>(hc);
	input.Bind(0,ControllerButton::ButtonB, killCommand, InputState::pressed);
	input.Bind(SDLK_q, killCommand, InputState::pressed);

	go = new GameObject();
	tlc = new QBert::TextureLineComponent(go, "Life.png");
	lc = new QBert::LivesComponent(go, tlc, 5);
	go->AddComponent(tlc);
	go->AddComponent(lc);
	go->GetComponentOfType<TransformComponent>()->SetPosition(200.f, 50.f);
	scene.Add(go);
	
	QBert = new GameObject();
	hc = new QBert::HealthComponent(QBert, QBert::HealthComponent::HealthOwner::QBert);
	hc->AddObserver(lc);
	QBert->AddComponent(hc);
	scene.Add(QBert);

	input.Bind(0,ControllerButton::ButtonX, std::make_shared<QBert::KillCommand>(hc), InputState::pressed);
	input.Bind(1,ControllerButton::ButtonX, std::make_shared<QBert::KillCommand>(hc), InputState::pressed);

	input.Bind(SDLK_e, std::make_shared<QBert::SoundCommand>(), InputState::pressed);
	
	std::cout << "Only controller is supported at the moment!\n";
	std::cout << "[Controller 0] A: Kill player 1\n";
	std::cout << "[Controller 0] B: Gain points\n";
	std::cout << "[Keyboard] Q: Gain points\n";
	std::cout << "[Controller 0] X: Kill player 2\n";
	std::cout << "[Controller 1] X: Kill player 2\n";
	std::cout << "[Keyboard] E: Make sound\n";
}

void dae::Minigin::Cleanup()
{
	endAudio();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();
	auto pSoundSystem = new SimpleSDL2AudioSoundSystem();
	ServiceLocator::RegisterSoundSystem(pSoundSystem);

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& time = Time::GetInstance();

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
