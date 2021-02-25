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

	input.Bind(ControllerButton::ButtonA, std::make_shared<QBert::KillCommand>(hc), InputState::pressed);

	GameObject* coily = new GameObject();
	hc = new QBert::HealthComponent(coily, QBert::HealthComponent::HealthOwner::Coily);
	hc->AddObserver(scoreComponent);
	coily->AddComponent(hc);
	scene.Add(coily);

	input.Bind(ControllerButton::ButtonB, std::make_shared<QBert::KillCommand>(hc), InputState::pressed);

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

	input.Bind(ControllerButton::ButtonX, std::make_shared<QBert::KillCommand>(hc), InputState::pressed);

	std::cout << "Only controller is supported at the moment!\n";
	std::cout << "A: Kill player 1\n";
	std::cout << "B: Gain points\n";
	std::cout << "X: Kill player 2\n";
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& time = Time::GetInstance();

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

	Cleanup();
}
