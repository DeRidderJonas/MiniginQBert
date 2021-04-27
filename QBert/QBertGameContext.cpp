#include "QBertGameContext.h"

#include <iostream>

#include "EnemyFactory.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "LivesDisplayComponent.h"
#include "MoveCommand.h"
#include "MovementComponent.h"
#include "MuteCommand.h"
#include "PlayableTerrainComponent.h"
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
}

void QBert::QBertGameContext::CreateLevel(QBert::ScoreComponent* pScoreComponent)
{
	float HexWidth{ 20.f };

	for (int r = 0; r < m_LevelWidth; ++r)
	{
		for (int q = 0; q < m_LevelWidth; ++q)
		{
			if (q < m_LevelWidth - r - 1)
				continue;

			auto go = new dae::GameObject();
			auto pTopTexture = new dae::TextureComponent(go);
			auto pLeftTexture = new dae::TextureComponent(go);
			auto pRightTexture = new dae::TextureComponent(go);
			go->AddComponent(pTopTexture);
			go->AddComponent(pLeftTexture);
			go->AddComponent(pRightTexture);
			auto play = new QBert::PlayableTerrainComponent(go, PlayableTerrainComponent::TerrainType::Normal, pTopTexture, pLeftTexture, pRightTexture);
			play->AddObserver(pScoreComponent);
			go->AddComponent(play);

			m_GameObjects[r][q] = go;
			float x{ sqrtf(3) * static_cast<float>(q) + sqrtf(3) / 2.f * static_cast<float>(r) };
			float y{ 3.f / 2.f * static_cast<float>(r) };
			go->GetComponentOfType<dae::TransformComponent>()->SetPosition(HexWidth * x + 80.f, HexWidth * y + 110.f);
			m_pScene->Add(go);
		}
	}
}

dae::GameObject* QBert::QBertGameContext::GetPlatform(int row, int col) const
{
	if (row >= m_LevelWidth || row < 0 || col >= m_LevelWidth || col < 0) return nullptr;

	return m_GameObjects[row][col];
}

void QBert::QBertGameContext::GetPlatformForGameObject(dae::GameObject* pToFind, int& row, int& col)
{
	for (int r = 0; r < m_LevelWidth; ++r)
	{
		for (int c = 0; c < m_LevelWidth; ++c)
		{
			if (m_GameObjects[r][c] == pToFind)
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
	return m_GameObjects[0][m_LevelWidth - 1];
}

bool QBert::QBertGameContext::IsPlatformOnBottom(dae::GameObject* pGo) const
{
	for (int i = 0; i < m_LevelWidth; ++i)
	{
		if (m_GameObjects[m_LevelWidth - 1][i] == pGo) return true;
	}

	return false;
}

int QBert::QBertGameContext::GetLevelWidth() const
{
	return m_LevelWidth;
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
	auto renderComponent = new dae::TextureComponent(m_pPlayer, "QBert.png");
	hc->AddObserver(lc);
	m_pPlayer->AddComponent(hc);
	m_pPlayer->AddComponent(mc);
	m_pPlayer->AddComponent(renderComponent);
	m_pScene->Add(m_pPlayer);

	//Input
	auto& input = dae::InputManager::GetInstance();
	
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

void QBert::QBertGameContext::Spawn(AIComponent::EnemyType enemyType, ScoreComponent* pScoreComponent)
{
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
