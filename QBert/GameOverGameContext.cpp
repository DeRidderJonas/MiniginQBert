#include "GameOverGameContext.h"

#include "GameObject.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TextComponent.h"
#include "TextureComponent.h"

QBert::GameOverGameContext::GameOverGameContext(dae::Scene* pScene)
	: GameContext(pScene)
	, m_score()
	, m_pTextComponent(nullptr)
{
	auto& scene = dae::SceneManager::GetInstance().GetScene("GameOver");
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto go = new dae::GameObject();
	auto renderComponent = new dae::TextureComponent(go);
	m_pTextScoreComponent = new dae::TextComponent(go, renderComponent, "Score: ", font);
	go->AddComponent(renderComponent);
	go->AddComponent(m_pTextScoreComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(10, 100.f);
	scene.Add(go);

	go = new dae::GameObject();
	renderComponent = new dae::TextureComponent(go);
	m_pTextComponent = new dae::TextComponent(go, renderComponent, "00000", font);
	go->AddComponent(renderComponent);
	go->AddComponent(m_pTextComponent);
	go->GetComponentOfType<dae::TransformComponent>()->SetPosition(80, 100.f);
	scene.Add(go);
}

QBert::GameOverGameContext::~GameOverGameContext()
{
	m_pTextScoreComponent->GetOwner()->Destroy();
	m_pTextComponent->GetOwner()->Destroy();
}

void QBert::GameOverGameContext::OnAddGameObject(dae::GameObject*)
{
}

void QBert::GameOverGameContext::OnRemoveGameObject(dae::GameObject* pGo)
{
	if (pGo == m_pTextScoreComponent->GetOwner())
		m_pTextScoreComponent = nullptr;

	if (pGo == m_pTextComponent->GetOwner())
		m_pTextComponent = nullptr;
}

void QBert::GameOverGameContext::Update()
{
}

void QBert::GameOverGameContext::SetScore(int score)
{
	m_score = score;
	
	char buffer[10]{};
	sprintf_s(buffer, "%05d", m_score);
	m_pTextComponent->SetText(buffer);
}
