#include <MiniginPCH.h>
#include <Minigin.h>
#include <vld.h>

#include <iostream>


#include "AIComponent.h"
#include "EnemyManager.h"
#include "EnemySpawnerComponent.h"
#include "FPSComponent.h"
#include "GameModeMenuComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "KillCommand.h"
#include "LevelManager.h"
#include "LivesDisplayComponent.h"
#include "MoveCommand.h"
#include "MovementComponent.h"
#include "MuteCommand.h"
#include "PlayableTerrainComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "SoundCommand.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TextureLineComponent.h"

using namespace QBert;

void LoadGame()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("QBert");
	auto& input = dae::InputManager::GetInstance();

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
	LevelManager::GetInstance().CreateLevel(scene, scoreComponent);

	//Lives display
	go = new dae::GameObject();
	auto tlc = new TextureLineComponent(go, "QBert.png");
	auto lc = new LivesDisplayComponent(go, tlc, 5);
	go->AddComponent(tlc);
	go->AddComponent(lc);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(200.f, 50.f);
	scene.Add(go);

	//Player
	auto QBert = new dae::GameObject();
	auto hc = new HealthComponent(QBert, HealthComponent::HealthOwner::QBert, 5);
	auto mc = new MovementComponent(QBert);
	renderComponent = new dae::TextureComponent(QBert, "QBert.png");
	hc->AddObserver(lc);
	QBert->AddComponent(hc);
	QBert->AddComponent(mc);
	QBert->AddComponent(renderComponent);
	scene.Add(QBert);

	EnemyManager::GetInstance().SetPlayer(QBert);

	//Enemy spawner
	go = new dae::GameObject();
	auto esc = new EnemySpawnerComponent(go, scoreComponent, true);
	go->AddComponent(esc);
	scene.Add(go);

	//Input
	input.Bind('e', std::make_shared<SoundCommand>(), dae::InputState::pressed);
	input.Bind('r', std::make_shared<MuteCommand>(), dae::InputState::released);

	
	input.Bind('w', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::UP, true), dae::InputState::pressed);
	input.Bind('s', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::DOWN, true), dae::InputState::pressed);
	input.Bind('a', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::LEFT, true), dae::InputState::pressed);
	input.Bind('d', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::RIGHT, true), dae::InputState::pressed);
	
	std::cout << "[Keyboard] E: Make sound\n";
	std::cout << "[Keyboard] R: Mute sound\n";

	std::cout << "[Keyboard] W: Move up\n";
	std::cout << "[Keyboard] A: Move left\n";
	std::cout << "[Keyboard] S: Move down\n";
	std::cout << "[Keyboard] D: Move right\n";
	
}

int main(int, char* []) {
	dae::Minigin engine;
	engine.Run(LoadGame);
	return 0;
}