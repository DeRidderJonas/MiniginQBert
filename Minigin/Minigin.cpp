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
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "TestCommand.h"

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

	auto go = new GameObject();
	auto renderComponent = new RenderComponent(go, "logo.png");
	go->AddComponent(renderComponent);
	go->GetComponentOfType<RenderComponent>()->SetTexture("background.jpg");
	scene.Add(go);

	go = new GameObject();
	renderComponent = new RenderComponent(go, "logo.png");
	auto transform = go->GetComponentOfType<TransformComponent>();
	transform->SetPosition(216, 180);

	go->AddComponent(renderComponent);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = new GameObject();
	renderComponent = new RenderComponent(go, "Programming 4 Assignment", font);
	go->AddComponent(renderComponent);
	transform = go->GetComponentOfType<TransformComponent>();
	transform->SetPosition(80, 20);
	scene.Add(go);


	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	go = new GameObject();
	renderComponent = new RenderComponent(go, "FPS", font);
	go->AddComponent(renderComponent);
	auto fpsComponent = new FPSComponent(go, renderComponent);
	go->AddComponent(fpsComponent);
	transform = go->GetComponentOfType<TransformComponent>();
	transform->SetPosition(5, 5);
	scene.Add(go);
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

		auto pTestCommand = std::make_shared<TestCommand>();
		input.Bind(ControllerButton::ButtonA, pTestCommand, InputState::pressed);
		input.Bind(ControllerButton::ButtonB, pTestCommand, InputState::down);
		input.Bind(ControllerButton::ButtonX, pTestCommand, InputState::released);
		
		bool doContinue = true;
		float lag{ 0.f };
		float fixedUpdateDeltaTime{ MsPerFrame / 1'000.f }; //Divide by a thousand because it's in MilliSeconds
		
		time.Start();
		while (doContinue)
		{
			time.Update();
			float deltaTime = time.GetDeltaTime();
			lag += deltaTime;

			doContinue = input.ProcessInput();

			while (lag >= fixedUpdateDeltaTime)
			{
				sceneManager.FixedUpdate(); //Uses Time.FixedUpdateDeltaTime
				lag -= fixedUpdateDeltaTime;
			}
			sceneManager.Update(); //Uses Time.DeltaTime
			
			renderer.Render();

			auto sleepTime = duration_cast<duration<float>>(time.GetNow() + milliseconds(MsPerFrame) - high_resolution_clock::now());
			if (sleepTime.count() > 0.f) this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
