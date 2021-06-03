#include "MiniginPCH.h"
#include "GameModeMenuComponent.h"

#include "FPSComponent.h"
#include "imgui.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TextureComponent.h"

QBert::GameModeMenuComponent::GameModeMenuComponent(dae::GameObject* pOwner)
	: RenderComponent(pOwner)
{
}

void QBert::GameModeMenuComponent::Update()
{
}

void QBert::GameModeMenuComponent::Initialize()
{
}

void QBert::GameModeMenuComponent::Render() const
{
	ImGui::Begin("Modes");

	if (ImGui::Button("Single Player"))
		StartGame(QBertGameContext::GameMode::Single);

	if(ImGui::Button("Co-op"))
		StartGame(QBertGameContext::GameMode::Coop);
	
	if(ImGui::Button("Versus"))
		StartGame(QBertGameContext::GameMode::Versus);
	
	ImGui::End();
}

void QBert::GameModeMenuComponent::StartGame(QBertGameContext::GameMode gameMode) const
{
	auto& scene = dae::SceneManager::GetInstance().GetScene("QBert");

	//GameContext
	scene.SetGameContext(nullptr);
	auto pGameContext = new QBertGameContext(&scene, gameMode);
	scene.SetGameContext(pGameContext);
	pGameContext->CreateLevel();
	pGameContext->CreatePlayer();

	dae::SceneManager::GetInstance().SetActiveScene("QBert");
}