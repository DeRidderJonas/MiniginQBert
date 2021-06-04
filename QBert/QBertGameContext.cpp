#include "QBertGameContext.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "BackToMenuComponent.h"
#include "CoilyAIComponent.h"
#include "EnemyFactory.h"
#include "EnemySpawnerComponent.h"
#include "FPSComponent.h"
#include "GameObject.h"
#include "GameOverGameContext.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "LivesDisplayComponent.h"
#include "MoveCommand.h"
#include "MovementComponent.h"
#include "MuteCommand.h"
#include "PlayableTerrainComponent.h"
#include "PlayableTerrainFactory.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ScoreComponent.h"
#include "ScoreEvent.h"
#include "SoundCommand.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "TextureLineComponent.h"

QBert::QBertGameContext::QBertGameContext(dae::Scene* pScene, GameMode gameMode)
	: GameContext(pScene)
	, m_gameMode(gameMode)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	//Background
	auto go = new dae::GameObject();
	auto renderComponent = new dae::TextureComponent(go, "background.jpg");
	go->AddComponent(renderComponent);
	m_pScene->Add(go);

	//FPS
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	auto textComponent = new dae::TextComponent(go, renderComponent, "FPS", font);
	auto fpsComponent = new dae::FPSComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(fpsComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(5, 5);
	m_pScene->Add(go);

	go = new dae::GameObject();
	auto bmc = new BackToMenuComponent(go);
	go->AddComponent(bmc);
	m_pScene->Add(go);
	
	//Score display
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	textComponent = new dae::TextComponent(go, renderComponent, "", font);
	m_pScoreComponent = new ScoreComponent(go, textComponent);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->AddComponent(m_pScoreComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(70.f, 100.f);
	m_pScene->Add(go);

	//Text
	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	textComponent = new dae::TextComponent(go, renderComponent, "Score: ", font);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(10, 100.f);
	m_pScene->Add(go);

	//Enemy spawner
	go = new dae::GameObject();
	m_pSpawnerComponent = new EnemySpawnerComponent(go, m_pScoreComponent, true);
	go->AddComponent(m_pSpawnerComponent);
	m_pScene->Add(go);

	SetScriptedEnemies();

	//Input
	auto& input = dae::InputManager::GetInstance();
	input.ClearInputs();
}

QBert::QBertGameContext::~QBertGameContext()
{
	m_pScene->DestroyAll();
}

void QBert::QBertGameContext::OnAddGameObject(dae::GameObject* )
{
}

void QBert::QBertGameContext::OnRemoveGameObject(dae::GameObject* pGameObject)
{
	m_Enemies.erase(std::remove(m_Enemies.begin(), m_Enemies.end(), pGameObject), m_Enemies.end());

	for (int row = 0; row < m_LevelHeight; ++row)
	{
		for (int col = 0; col < m_LevelWidth; ++col)
		{
			if (m_PlayableGrid[row][col] == pGameObject) 
				m_PlayableGrid[row][col] = nullptr;
		}
	}

	if(m_gameMode == GameMode::Versus && pGameObject == m_pPlayerTwo)
	{
		m_pPlayerTwo = nullptr;

		auto& input = dae::InputManager::GetInstance();

		input.Unbind('i');
		input.Unbind('k');
		input.Unbind('j');
		input.Unbind('l');
	}
}

void QBert::QBertGameContext::Update()
{
	CheckCollisions();
	CheckPlatforms();
	CheckGameOverConditions();
}

bool QBert::QBertGameContext::CreateLevel()
{
	std::string levelLayout{ dae::ResourceManager::GetInstance().LoadFile("Level" + std::to_string(m_CurrentLevel) + ".txt") };

	if (levelLayout.empty())
		return false;
	
	float HexWidth{ 17.f };
	float startX{ 80.f }, startY{ 110.f };

	int row{}, col{};
	
	std::stringstream ss{ levelLayout };
	std::string rowData{};
	while(std::getline(ss, rowData))
	{
		for(auto platformType : rowData)
		{
			auto pGo = PlayableTerrainFactory::CreatePlatform(platformType, m_pScoreComponent, row, col, HexWidth, startX, startY);
			m_PlayableGrid[row][col] = pGo;
			col++;
			if(pGo) m_pScene->Add(pGo);
		}
		row++;
		col = 0;
	}

	return true;
}

dae::GameObject* QBert::QBertGameContext::GetPlatform(int row, int col) const
{
	if (row >= m_LevelHeight || row < 0 || col >= m_LevelWidth || col < 0) return nullptr;

	return m_PlayableGrid[row][col];
}

void QBert::QBertGameContext::GetPlatformForGameObject(dae::GameObject* pToFind, int& row, int& col)
{
	for (int r = 0; r < m_LevelHeight; ++r)
	{
		for (int c = 0; c < m_LevelWidth; ++c)
		{
			if (m_PlayableGrid[r][c] == pToFind)
			{
				row = r;
				col = c;
				return;
			}
		}
	}
}

dae::GameObject* QBert::QBertGameContext::GetSpawnPlatform(bool isPlayer, bool playerTwo) const
{
	if(isPlayer)
	{
		switch (m_gameMode)
		{
		case GameMode::Coop: 
			if (playerTwo)
				return m_PlayableGrid[m_LevelHeight - 1][m_LevelWidth - 2];
			else
				return m_PlayableGrid[m_LevelHeight - 1][1];
		case GameMode::Single:
		case GameMode::Versus:
		default:
			return m_PlayableGrid[0][m_LevelWidth - 2];
		}
	}
	else
		return m_PlayableGrid[0][m_LevelWidth - 2];
}

bool QBert::QBertGameContext::IsPlatformOnBottom(dae::GameObject* pGo) const
{
	for (int i = 0; i < m_LevelWidth; ++i)
	{
		if (m_PlayableGrid[m_LevelHeight - 1][i] == pGo) return true;
	}

	return false;
}

int QBert::QBertGameContext::GetLevelWidth() const
{
	return m_LevelWidth;
}

int QBert::QBertGameContext::GetLevelHeight() const
{
	return m_LevelHeight;
}

void QBert::QBertGameContext::GetEnemyPlayableRange(int& rowMin, int& rowMax, int& colMin, int& colMax) const
{
	rowMin = 0;
	rowMax = m_LevelHeight - 1;
	colMin = 1;
	colMax = m_LevelWidth - 2;
}

void QBert::QBertGameContext::CreatePlayer()
{
	const int amountOfPlayerLives{ 3 };
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	
	//Lives display
	auto go = new dae::GameObject();
	auto tlc = new TextureLineComponent(go, "QBert.png");
	auto lc = new LivesDisplayComponent(go, tlc, amountOfPlayerLives);
	go->AddComponent(tlc);
	go->AddComponent(lc);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(100.f, 50.f);
	m_pScene->Add(go);

	//Text
	go = new dae::GameObject();
	auto renderComponent = new dae::TextureComponent(go);
	auto textComponent = new dae::TextComponent(go, renderComponent, "Player 1:", font);
	go->AddComponent(renderComponent);
	go->AddComponent(textComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(10.f, 50.f);
	m_pScene->Add(go);
	
	//Player
	m_pPlayer = new dae::GameObject();
	auto hc = new HealthComponent(m_pPlayer, HealthComponent::HealthOwner::QBert, amountOfPlayerLives);
	auto mc = new MovementComponent(m_pPlayer, GetSpawnPlatform(true));
	renderComponent = new dae::TextureComponent(m_pPlayer, "QBert.png", 1);
	hc->AddObserver(lc);
	m_pPlayer->AddComponent(hc);
	m_pPlayer->AddComponent(mc);
	m_pPlayer->AddComponent(renderComponent);
	m_pScene->Add(m_pPlayer);

	//Input
	auto& input = dae::InputManager::GetInstance();
	
	input.Bind('e', std::make_shared<SoundCommand>(), dae::InputState::pressed);
	input.Bind('r', std::make_shared<MuteCommand>(), dae::InputState::released);

	auto upC = std::make_shared<MoveCommand>(mc, MovementComponent::Direction::UP, true, false, true);
	auto downC = std::make_shared<MoveCommand>(mc, MovementComponent::Direction::DOWN, true, false, true);
	auto leftC = std::make_shared<MoveCommand>(mc, MovementComponent::Direction::LEFT, true, false, true);
	auto rightC = std::make_shared<MoveCommand>(mc, MovementComponent::Direction::RIGHT, true, false, true);
	
	input.Bind('w', upC, dae::InputState::pressed);
	input.Bind('s', downC, dae::InputState::pressed);
	input.Bind('a', leftC, dae::InputState::pressed);
	input.Bind('d', rightC, dae::InputState::pressed);

	input.Bind(0, dae::ControllerButton::DpadUp, upC, dae::InputState::pressed);
	input.Bind(0, dae::ControllerButton::DpadDown, downC, dae::InputState::pressed);
	input.Bind(0, dae::ControllerButton::DpadLeft, leftC, dae::InputState::pressed);
	input.Bind(0, dae::ControllerButton::DpadRight, rightC, dae::InputState::pressed);

	std::cout << "[Keyboard] E: Make sound\n";
	std::cout << "[Keyboard] R: Mute sound\n";

	std::cout << "--- Player 1 ---\n";
	std::cout << "[Keyboard] W: Move up\n";
	std::cout << "[Keyboard] A: Move left\n";
	std::cout << "[Keyboard] S: Move down\n";
	std::cout << "[Keyboard] D: Move right\n\n";

	std::cout << "[Controller] Dpad Up: Move up\n";
	std::cout << "[Controller] Dpad Left: Move left\n";
	std::cout << "[Controller] Dpad Down: Move down\n";
	std::cout << "[Controller] Dpad Right: Move right\n\n\n";

	if(m_gameMode == GameMode::Coop)
	{
		//Lives
		go = new dae::GameObject();
		tlc = new TextureLineComponent(go, "QBert.png");
		lc = new LivesDisplayComponent(go, tlc, amountOfPlayerLives);
		go->AddComponent(tlc);
		go->AddComponent(lc);
		go->GetComponentOfType<dae::TransformComponent>()->SetPosition(450.f, 50.f);
		m_pScene->Add(go);

		//Text
		go = new dae::GameObject();
		renderComponent = new dae::TextureComponent(go);
		textComponent = new dae::TextComponent(go, renderComponent, "Player 2:", font);
		go->AddComponent(renderComponent);
		go->AddComponent(textComponent);
		go->GetComponentOfType<dae::TransformComponent>()->SetPosition(360.f, 50.f);
		m_pScene->Add(go);
		
		//Player
		m_pPlayerTwo = new dae::GameObject();
		hc = new HealthComponent(m_pPlayerTwo, HealthComponent::HealthOwner::QBert, amountOfPlayerLives);
		mc = new MovementComponent(m_pPlayerTwo, GetSpawnPlatform(true, true));
		renderComponent = new dae::TextureComponent(m_pPlayerTwo, "QBert.png", 1);
		hc->AddObserver(lc);
		m_pPlayerTwo->AddComponent(hc);
		m_pPlayerTwo->AddComponent(mc);
		m_pPlayerTwo->AddComponent(renderComponent);
		m_pScene->Add(m_pPlayerTwo);

		//Input
		upC = std::make_shared<MoveCommand>(mc, MovementComponent::Direction::UP, true, false, true);
		downC = std::make_shared<MoveCommand>(mc, MovementComponent::Direction::DOWN, true, false, true);
		leftC = std::make_shared<MoveCommand>(mc, MovementComponent::Direction::LEFT, true, false, true);
		rightC = std::make_shared<MoveCommand>(mc, MovementComponent::Direction::RIGHT, true, false, true);
		
		input.Bind('i', upC, dae::InputState::pressed);
		input.Bind('k', downC, dae::InputState::pressed);
		input.Bind('j', leftC, dae::InputState::pressed);
		input.Bind('l', rightC, dae::InputState::pressed);

		input.Bind(1, dae::ControllerButton::DpadUp, upC, dae::InputState::pressed);
		input.Bind(1, dae::ControllerButton::DpadDown, downC, dae::InputState::pressed);
		input.Bind(1, dae::ControllerButton::DpadLeft, leftC, dae::InputState::pressed);
		input.Bind(1, dae::ControllerButton::DpadRight, rightC, dae::InputState::pressed);

		std::cout << "--- Player 2 ---\n";
		std::cout << "[Keyboard] I: Move up\n";
		std::cout << "[Keyboard] J: Move left\n";
		std::cout << "[Keyboard] K: Move down\n";
		std::cout << "[Keyboard] L: Move right\n\n";

		std::cout << "[Controller] Dpad Up: Move up\n";
		std::cout << "[Controller] Dpad Left: Move left\n";
		std::cout << "[Controller] Dpad Down: Move down\n";
		std::cout << "[Controller] Dpad Right: Move right\n\n\n";
	}
	if(m_gameMode == GameMode::Versus)
	{
		SpawnVersusCoily();

		std::cout << "--- Player 2 (Once the egg reaches the bottom)---\n";
		std::cout << "[Keyboard] I: Move up\n";
		std::cout << "[Keyboard] J: Move left\n";
		std::cout << "[Keyboard] K: Move down\n";
		std::cout << "[Keyboard] L: Move right\n\n";

		std::cout << "[Controller] Dpad Up: Move up\n";
		std::cout << "[Controller] Dpad Left: Move left\n";
		std::cout << "[Controller] Dpad Down: Move down\n";
		std::cout << "[Controller] Dpad Right: Move right\n\n\n";
	}
}

void QBert::QBertGameContext::Spawn(AIComponent::EnemyType enemyType, ScoreComponent* pScoreComponent)
{
	if (!m_pPlayer)
		return;

	if(enemyType == AIComponent::EnemyType::Coily)
	{
		bool coilyExists = std::any_of(m_Enemies.begin(), m_Enemies.end(), [](dae::GameObject* pGo)
			{
				auto pCoilyAI = pGo->GetComponentOfType<CoilyAIComponent>();
				return pCoilyAI != nullptr;
			});
		if (coilyExists)
			return;
	}
	
	auto newEnemy = EnemyFactory::CreateEnemy(enemyType, pScoreComponent, this, m_pPlayer);
	dae::Scene& scene = dae::SceneManager::GetInstance().GetScene("QBert");
	
	m_Enemies.push_back(newEnemy);
	scene.Add(newEnemy);
}

std::vector<dae::GameObject*>& QBert::QBertGameContext::GetEnemies()
{
	return m_Enemies;
}

void QBert::QBertGameContext::CheckCollisions()
{
	CheckCollisionForPlayer(m_pPlayer);

	if (m_gameMode == GameMode::Coop)
		CheckCollisionForPlayer(m_pPlayerTwo);
}

void QBert::QBertGameContext::CheckPlatforms()
{
	for(auto& row : m_PlayableGrid)
	{
		for(auto pGo : row)
		{
			if (!pGo) continue;

			auto pPlayableTerrain = pGo->GetComponentOfType<PlayableTerrainComponent>();
			if (!pPlayableTerrain->IsFullyActivated())
				return;
		}
	}

	GoToNextLevel();
}

void QBert::QBertGameContext::CheckGameOverConditions()
{
	switch (m_gameMode)
	{
	case GameMode::Coop:
		if (!m_pPlayer && !m_pPlayerTwo)
			GoToGameOver();
		break;
	case GameMode::Single: 
	case GameMode::Versus: 
	default:
		if (!m_pPlayer)
			GoToGameOver();
		break;
	}
}

void QBert::QBertGameContext::SetScriptedEnemies()
{
	//https://gamefaqs.gamespot.com/nes/587540-qbert/faqs/64992
	std::queue<AIComponent::EnemyType> scripted{};
	switch (m_CurrentLevel)
	{
	case 1:
		scripted.push(AIComponent::EnemyType::Coily);
		scripted.push(AIComponent::EnemyType::SlickSam);
		scripted.push(AIComponent::EnemyType::SlickSam);
		scripted.push(AIComponent::EnemyType::SlickSam);
		m_pSpawnerComponent->SetScriptedSpawns(scripted);
		break;
	case 2:
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::Coily);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::Coily);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::Coily);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::Coily);
		m_pSpawnerComponent->SetScriptedSpawns(scripted);
		break;
	case 3:
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::SlickSam);
		scripted.push(AIComponent::EnemyType::Coily);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::SlickSam);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::UggWrongWay);
		scripted.push(AIComponent::EnemyType::SlickSam);
		scripted.push(AIComponent::EnemyType::SlickSam);
		scripted.push(AIComponent::EnemyType::Coily);
		m_pSpawnerComponent->SetScriptedSpawns(scripted);
		break;
	}
}

void QBert::QBertGameContext::GoToGameOver()
{
	auto& gameOverScene = dae::SceneManager::GetInstance().GetScene("GameOver");
	auto pGameOverContext = new GameOverGameContext(&gameOverScene);
	pGameOverContext->SetScore(m_pScoreComponent->GetScore());

	gameOverScene.SetGameContext(pGameOverContext);

	dae::SceneManager::GetInstance().SetActiveScene("GameOver");
}

void QBert::QBertGameContext::OnPlayerDestroy()
{
	if(m_pPlayer)
	{
		auto pHealthComponent = m_pPlayer->GetComponentOfType<HealthComponent>();
		if (pHealthComponent->IsDead())
		{
			m_pPlayer = nullptr;

			auto& input = dae::InputManager::GetInstance();

			input.Unbind('w');
			input.Unbind('s');
			input.Unbind('a');
			input.Unbind('d');
		}
	}

	if(m_gameMode == GameMode::Coop && m_pPlayerTwo)
	{
		auto pHealthComponent = m_pPlayerTwo->GetComponentOfType<HealthComponent>();
		if(pHealthComponent->IsDead())
		{
			m_pPlayerTwo = nullptr;

			auto& input = dae::InputManager::GetInstance();

			input.Unbind('i');
			input.Unbind('k');
			input.Unbind('j');
			input.Unbind('l');
		}
	}
}

void QBert::QBertGameContext::CheckCollisionForPlayer(dae::GameObject* pPlayer)
{
	if (!pPlayer || m_Enemies.empty())
		return;

	auto pPlayerMovementComponent = pPlayer->GetComponentOfType<MovementComponent>();
	auto pPlayerPlatform = pPlayerMovementComponent->GetPlatform();

	auto enemyIt = std::find_if(m_Enemies.begin(), m_Enemies.end(), [&pPlayerPlatform](dae::GameObject* pGo)
		{
			auto pMovementComponent = pGo->GetComponentOfType<MovementComponent>();
			return pPlayerPlatform == pMovementComponent->GetPlatform();
		});

	if (enemyIt != m_Enemies.end())
	{
		//Player and enemy on same platform
		dae::GameObject* pEnemy = *enemyIt;
		auto pAIComponent = pEnemy->GetComponentOfType<AIComponent>();
		if (pAIComponent) pAIComponent->OnCollisionWithPlayer(pPlayer);
	}
}

void QBert::QBertGameContext::GoToNextLevel()
{
	if (!m_pPlayer) 
		return;
	
	KillEnemies();
	DestroyLevel();

	m_CurrentLevel++;
	if (CreateLevel())
	{
		m_pPlayer->GetComponentOfType<MovementComponent>()->GoToSpawningPlatform(true);
		
		if (m_gameMode == GameMode::Coop && m_pPlayerTwo)
			m_pPlayerTwo->GetComponentOfType<MovementComponent>()->GoToSpawningPlatform(true, true);
		if(m_gameMode == GameMode::Versus)
		{
			if (m_pPlayerTwo)
				m_pPlayerTwo->Destroy();

			SpawnVersusCoily();
		}

		SetScriptedEnemies();
	}
	else
	{
		GoToGameOver();
		
		m_pPlayer->Destroy();
		OnPlayerDestroy();
	}
}

void QBert::QBertGameContext::KillEnemies()
{
	for(auto pEnemy : m_Enemies)
	{
		pEnemy->Destroy();
	}
	m_Enemies.clear();
}

void QBert::QBertGameContext::DestroyLevel()
{
	for (int row = 0; row < m_LevelHeight; ++row)
	{
		for (int col = 0; col < m_LevelWidth; ++col)
		{
			auto pGo = m_PlayableGrid[row][col];
			if (!pGo) continue;

			auto pPlayableTerrainComponent = pGo->GetComponentOfType<PlayableTerrainComponent>();
			if(pPlayableTerrainComponent)
			{
				if(pPlayableTerrainComponent->GetType() == PlayableTerrainComponent::TerrainType::Disc)
				{
					ScoreEvent event{ "SCORE", 50 };
					m_pScoreComponent->OnNotify(event);
				}
			}
			
			pGo->Destroy();
			m_PlayableGrid[row][col] = nullptr;
		}
	}
}

void QBert::QBertGameContext::SpawnVersusCoily()
{
	auto& input = dae::InputManager::GetInstance();

	input.Unbind('i');
	input.Unbind('k');
	input.Unbind('j');
	input.Unbind('l');
	
	m_pPlayerTwo = EnemyFactory::CreateEnemy(AIComponent::EnemyType::Coily, m_pScoreComponent, this, m_pPlayer, false);
	dae::Scene& scene = dae::SceneManager::GetInstance().GetScene("QBert");

	m_Enemies.push_back(m_pPlayerTwo);
	scene.Add(m_pPlayerTwo);
}
