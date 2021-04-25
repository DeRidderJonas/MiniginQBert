#include <MiniginPCH.h>
#include <Minigin.h>


#include "EnemySpawnerComponent.h"
#include "FPSComponent.h"
#include "GameModeMenuComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "PlayableTerrainComponent.h"
#include "QBertGameContext.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "TextureComponent.h"

using namespace QBert;

void LoadGame()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("QBert");

	//Background
	auto go = new dae::GameObject();
	auto renderComponent = new dae::TextureComponent(go, "logo.png");
	go->AddComponent(renderComponent);
	go->GetComponentOfType<dae::TextureComponent>()->SetTexture("background.jpg");
	scene.Add(go);

	//FPS
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	auto textComponent = new dae::TextComponent(go, renderComponent, "FPS", font);
	auto fpsComponent = new dae::FPSComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(fpsComponent);
	auto transform = go->GetComponentOfType<dae::TransformComponent>();
	transform->SetPosition(5, 5);
	scene.Add(go);

	//Gamemode menu component
	go = new dae::GameObject();
	GameModeMenuComponent* gmmc = new GameModeMenuComponent(go);
	go->AddComponent(gmmc);
	scene.Add(go);

	//Score display
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	textComponent = new dae::TextComponent(go, renderComponent, "", font);
	auto scoreComponent = new ScoreComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(scoreComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(5.f, 100.f);
	scene.Add(go);

	//Create level
	auto pGameContext = new QBertGameContext(&scene);
	scene.SetGameContext(pGameContext);
	pGameContext->CreateLevel(scoreComponent);
	pGameContext->CreatePlayer();

	//Enemy spawner
	go = new dae::GameObject();
	auto esc = new EnemySpawnerComponent(go, scoreComponent, true);
	go->AddComponent(esc);
	scene.Add(go);
	
}

int main(int, char* []) {
	dae::Minigin engine;
	engine.Run(LoadGame);
	return 0;
}