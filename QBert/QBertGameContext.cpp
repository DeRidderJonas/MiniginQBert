#include "QBertGameContext.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "EnemyFactory.h"
#include "GameObject.h"
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
#include "SoundCommand.h"
#include "TextureComponent.h"
#include "TextureLineComponent.h"

QBert::QBertGameContext::QBertGameContext(dae::Scene* pScene)
	: GameContext(pScene)
{
}

void QBert::QBertGameContext::OnAddGameObject(dae::GameObject* )
{
}

void QBert::QBertGameContext::OnRemoveGameObject(dae::GameObject* pGameObject)
{
	m_Enemies.erase(std::remove(m_Enemies.begin(), m_Enemies.end(), pGameObject), m_Enemies.end());
}

void QBert::QBertGameContext::Update()
{
	CheckCollisions();
	CheckPlatforms();
}

bool QBert::QBertGameContext::CreateLevel(QBert::ScoreComponent* pScoreComponent)
{
	std::string levelLayout{ dae::ResourceManager::GetInstance().LoadFile("Level" + std::to_string(m_CurrentLevel) + ".txt") };

	if (levelLayout.empty())
		return false;
	
	m_pScoreComponent = pScoreComponent;
	
	float HexWidth{ 20.f };
	float startX{ 80.f }, startY{ 110.f };

	int row{}, col{};
	
	std::stringstream ss{ levelLayout };
	std::string rowData{};
	while(std::getline(ss, rowData))
	{
		for(auto platformType : rowData)
		{
			auto pGo = PlayableTerrainFactory::CreatePlatform(platformType, pScoreComponent, row, col, HexWidth, startX, startY);
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

dae::GameObject* QBert::QBertGameContext::GetSpawnPlatform() const
{
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

void QBert::QBertGameContext::GetEnemyPlayableRange(int& rowMin, int& rowMax, int& colMin, int& colMax) const
{
	rowMin = 0;
	rowMax = m_LevelHeight - 1;
	colMin = 1;
	colMax = m_LevelWidth - 2;
}

void QBert::QBertGameContext::CreatePlayer()
{
	//Lives display
	auto go = new dae::GameObject();
	auto tlc = new TextureLineComponent(go, "QBert.png");
	auto lc = new LivesDisplayComponent(go, tlc, 5);
	go->AddComponent(tlc);
	go->AddComponent(lc);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(200.f, 50.f);
	m_pScene->Add(go);

	//Player
	m_pPlayer = new dae::GameObject();
	auto hc = new HealthComponent(m_pPlayer, HealthComponent::HealthOwner::QBert, 5);
	auto mc = new MovementComponent(m_pPlayer);
	auto renderComponent = new dae::TextureComponent(m_pPlayer, "QBert.png", 1);
	hc->AddObserver(lc);
	m_pPlayer->AddComponent(hc);
	m_pPlayer->AddComponent(mc);
	m_pPlayer->AddComponent(renderComponent);
	m_pScene->Add(m_pPlayer);

	//Input
	auto& input = dae::InputManager::GetInstance();
	
	input.Bind('e', std::make_shared<SoundCommand>(), dae::InputState::pressed);
	input.Bind('r', std::make_shared<MuteCommand>(), dae::InputState::released);

	input.Bind('w', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::UP, true, false, true), dae::InputState::pressed);
	input.Bind('s', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::DOWN, true, false, true), dae::InputState::pressed);
	input.Bind('a', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::LEFT, true, false, true), dae::InputState::pressed);
	input.Bind('d', std::make_shared<MoveCommand>(mc, MovementComponent::Direction::RIGHT, true, false, true), dae::InputState::pressed);

	std::cout << "[Keyboard] E: Make sound\n";
	std::cout << "[Keyboard] R: Mute sound\n";

	std::cout << "[Keyboard] W: Move up\n";
	std::cout << "[Keyboard] A: Move left\n";
	std::cout << "[Keyboard] S: Move down\n";
	std::cout << "[Keyboard] D: Move right\n";
}

void QBert::QBertGameContext::Spawn(AIComponent::EnemyType enemyType, ScoreComponent* pScoreComponent)
{
	if (!m_pPlayer)
		return;
	
	auto newEnemy = EnemyFactory::CreateEnemy(enemyType, pScoreComponent, this, m_pPlayer);
	dae::Scene& scene = dae::SceneManager::GetInstance().GetScene("QBert");
	
	m_Enemies.push_back(newEnemy);
	scene.Add(newEnemy);
}

void QBert::QBertGameContext::CheckCollisions()
{
	if (!m_pPlayer || m_Enemies.empty())
		return;
	
	auto pPlayerMovementComponent = m_pPlayer->GetComponentOfType<MovementComponent>();
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
		if (pAIComponent) pAIComponent->OnCollisionWithPlayer(m_pPlayer);
	}
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

void QBert::QBertGameContext::OnPlayerDestroy()
{
	m_pPlayer = nullptr;

	auto& input = dae::InputManager::GetInstance();

	input.Unbind('w');
	input.Unbind('s');
	input.Unbind('a');
	input.Unbind('d');
}

void QBert::QBertGameContext::GoToNextLevel()
{
	if (!m_pPlayer) 
		return;
	
	KillEnemies();
	DestroyLevel();

	m_CurrentLevel++;
	if (CreateLevel(m_pScoreComponent))
		m_pPlayer->GetComponentOfType<MovementComponent>()->GoToSpawningPlatform();
	else
	{
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

			pGo->Destroy();
			m_PlayableGrid[row][col] = nullptr;
		}
	}
}
