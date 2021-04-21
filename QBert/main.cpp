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

	auto go = new dae::GameObject();
	auto renderComponent = new dae::TextureComponent(go, "logo.png");
	go->AddComponent(renderComponent);
	go->GetComponentOfType<dae::TextureComponent>()->SetTexture("background.jpg");
	scene.Add(go);
	
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go, "logo.png");
	auto transform = go->GetComponentOfType<dae::TransformComponent>();
	transform->SetPosition(216, 180);

	go->AddComponent(renderComponent);
	scene.Add(go);


	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	auto textComponent = new dae::TextComponent(go, renderComponent, "Programming 4 Assignment", font);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	transform = go->GetComponentOfType<dae::TransformComponent>();
	transform->SetPosition(80, 20);
	scene.Add(go);


	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	textComponent = new dae::TextComponent(go, renderComponent, "FPS", font);
	auto fpsComponent = new dae::FPSComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(fpsComponent);
	transform = go->GetComponentOfType<dae::TransformComponent>();
	transform->SetPosition(5, 5);
	scene.Add(go);

	go = new dae::GameObject();
	GameModeMenuComponent* gmmc = new GameModeMenuComponent(go);
	go->AddComponent(gmmc);
	scene.Add(go);


	LevelManager::GetInstance().CreateLevel(scene);

	go = new dae::GameObject();
	TextureLineComponent* tlc = new TextureLineComponent(go, "QBert.png");
	LivesDisplayComponent* lc = new LivesDisplayComponent(go, tlc, 5);
	go->AddComponent(tlc);
	go->AddComponent(lc);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(5.f, 50.f);
	scene.Add(go);


	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	textComponent = new dae::TextComponent(go, renderComponent, "", font);
	auto scoreComponent = new ScoreComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(scoreComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(5.f, 100.f);
	scene.Add(go);

	auto QBert = new dae::GameObject();
	HealthComponent* hc = new HealthComponent(QBert, HealthComponent::HealthOwner::QBert, 5);
	hc->AddObserver(lc);
	QBert->AddComponent(hc);
	scene.Add(QBert);

	input.Bind(0, dae::ControllerButton::ButtonA, std::make_shared<KillCommand>(hc), dae::InputState::pressed);

	/*auto coily = new dae::GameObject();
	hc = new HealthComponent(coily, HealthComponent::HealthOwner::Coily, 1);
	hc->AddObserver(scoreComponent);
	auto mcC = new MovementComponent(coily);
	renderComponent = new dae::TextureComponent(coily, "Coily.png");
	auto aiC = new AIComponent(coily, AIComponent::EnemyType::Coily, mcC);
	coily->AddComponent(hc);
	coily->AddComponent(mcC);
	coily->AddComponent(renderComponent);
	coily->AddComponent(aiC);
	scene.Add(coily);*/

	
	auto killCommand = std::make_shared<QBert::KillCommand>(hc);
	input.Bind(0, dae::ControllerButton::ButtonB, killCommand, dae::InputState::pressed);
	input.Bind('q', killCommand, dae::InputState::pressed);

	go = new dae::GameObject();
	tlc = new TextureLineComponent(go, "QBert.png");
	lc = new LivesDisplayComponent(go, tlc, 5);
	go->AddComponent(tlc);
	go->AddComponent(lc);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(200.f, 50.f);
	scene.Add(go);

	QBert = new dae::GameObject();
	hc = new HealthComponent(QBert, HealthComponent::HealthOwner::QBert, 5);
	auto mc = new MovementComponent(QBert);
	renderComponent = new dae::TextureComponent(QBert, "QBert.png");
	hc->AddObserver(lc);
	QBert->AddComponent(hc);
	QBert->AddComponent(mc);
	QBert->AddComponent(renderComponent);
	scene.Add(QBert);

	EnemyManager::GetInstance().SetPlayer(QBert);

	go = new dae::GameObject();
	auto esc = new EnemySpawnerComponent(go, scoreComponent, true);
	go->AddComponent(esc);
	scene.Add(go);

	input.Bind(0, dae::ControllerButton::ButtonX, std::make_shared<QBert::KillCommand>(hc), dae::InputState::pressed);
	input.Bind(1, dae::ControllerButton::ButtonX, std::make_shared<QBert::KillCommand>(hc), dae::InputState::pressed);

	input.Bind('e', std::make_shared<SoundCommand>(), dae::InputState::pressed);
	input.Bind('r', std::make_shared<MuteCommand>(), dae::InputState::released);

	
	std::cout << "Only controller is supported at the moment!\n";
	std::cout << "[Controller 0] A: Kill player 1\n";
	std::cout << "[Controller 0] B: Gain points\n";
	std::cout << "[Keyboard] Q: Gain points\n";
	std::cout << "[Controller 0] X: Kill player 2\n";
	std::cout << "[Controller 1] X: Kill player 2\n";
	std::cout << "[Keyboard] E: Make sound\n";

	std::cout << "[Keyboard] W: Move up\n";
	std::cout << "[Keyboard] A: Move left\n";
	std::cout << "[Keyboard] S: Move down\n";
	std::cout << "[Keyboard] D: Move right\n";
	
	input.Bind('w', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::UP, true), dae::InputState::pressed);
	input.Bind('s', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::DOWN, true), dae::InputState::pressed);
	input.Bind('a', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::LEFT, true), dae::InputState::pressed);
	input.Bind('d', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::RIGHT, true), dae::InputState::pressed);
	
}

int main(int, char* []) {
	dae::Minigin engine;
	engine.Run(LoadGame);
	return 0;
}