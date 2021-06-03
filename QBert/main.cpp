#include <MiniginPCH.h>
#include <Minigin.h>

#include <vld.h>

#include "BackToMenuComponent.h"
#include "FPSComponent.h"
#include "GameModeMenuComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "PlayableTerrainComponent.h"
#include "QBertGameContext.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TextComponent.h"
#include "TextureComponent.h"

using namespace QBert;

void LoadGame()
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//----- Menu Scene -----
	auto& MenuScene = dae::SceneManager::GetInstance().CreateScene("Menu");

	//Background
	auto go = new dae::GameObject();
	auto renderComponent = new dae::TextureComponent(go, "background.jpg");
	go->AddComponent(renderComponent);
	MenuScene.Add(go);

	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go, "logo.png");
	go->AddComponent(renderComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(220.f, 200.f);
	MenuScene.Add(go);
	
	//FPS
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	auto textComponent = new dae::TextComponent(go, renderComponent, "FPS", font);
	auto fpsComponent = new dae::FPSComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(fpsComponent);
	auto transform = go->GetComponentOfType<dae::TransformComponent>();
	transform->SetPosition(5, 5);
	MenuScene.Add(go);

	//Gamemode menu component
	go = new dae::GameObject();
	auto gmmc = new GameModeMenuComponent(go);
	go->AddComponent(gmmc);
	MenuScene.Add(go);

	//----- QBert Scene-----
	dae::SceneManager::GetInstance().CreateScene("QBert");
	
	//------ Game Over Scene -----
	auto& gameOverScene = dae::SceneManager::GetInstance().CreateScene("GameOver");

	//Background
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go, "background.jpg");
	go->AddComponent(renderComponent);
	gameOverScene.Add(go);

	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go, "logo.png");
	go->AddComponent(renderComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(220.f, 200.f);
	gameOverScene.Add(go);

	//FPS
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	textComponent = new dae::TextComponent(go, renderComponent, "FPS", font);
	fpsComponent = new dae::FPSComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(fpsComponent);
	transform = go->GetComponentOfType<dae::TransformComponent>();
	transform->SetPosition(5, 5);
	gameOverScene.Add(go);

	go = new dae::GameObject();
	auto bmc = new BackToMenuComponent(go);
	go->AddComponent(bmc);
	gameOverScene.Add(go);
}

int main(int, char* []) {
	dae::Minigin engine;
	engine.Run(LoadGame);
	return 0;
}