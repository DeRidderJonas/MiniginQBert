#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
//#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"
#include "Time.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"

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
	auto textureComponent = new TextureComponent(go, "logo.png");
	go->AddComponent(textureComponent);
	go->GetComponentOfType<TextureComponent>()->SetTexture("background.jpg");
	scene.Add(go);

	go = new GameObject();
	//go->GetComponentOfType<TextureComponent>()->SetTexture("logo.png");
	textureComponent = new TextureComponent(go, "logo.png");
	auto transform = go->GetComponentOfType<TransformComponent>();
	transform->SetPosition(216, 180);

	go->AddComponent(textureComponent);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = new GameObject();
	auto textComponent = new TextComponent(go, "Programming 4 Assignment", font);
	go->AddComponent(textComponent);
	transform = go->GetComponentOfType<TransformComponent>();
	transform->SetPosition(80, 20);
	scene.Add(go);


	font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	go = new GameObject();
	textComponent = new TextComponent(go, "FPS", font);
	go->AddComponent(textComponent);
	auto fpsComponent = new FPSComponent(go, textComponent);
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

		bool doContinue = true;
		float lag{ 0.f };
		Time::GetInstance().Start();
		while (doContinue)
		{
			Time::GetInstance().Update();
			float deltaTime = Time::GetInstance().GetDeltaTime();
			lag += deltaTime;
			doContinue = input.ProcessInput();
			while (lag >= (MsPerFrame / 1'000.f)) //Divide by a thousand because it's in MilliSeconds
			{
				sceneManager.Update();
				lag -= (MsPerFrame / 1'000.f);
			}
			renderer.Render();
		}
	}

	Cleanup();
}
