#include "BackToMenuComponent.h"

#include "imgui.h"
#include "SceneManager.h"

QBert::BackToMenuComponent::BackToMenuComponent(dae::GameObject* pOwner)
	: RenderComponent(pOwner)
{
}

void QBert::BackToMenuComponent::Initialize()
{
}

void QBert::BackToMenuComponent::Render() const
{
	ImGui::Begin("Back");

	if (ImGui::Button("Back"))
		GoToMainMenu();
	
	ImGui::End();
}

void QBert::BackToMenuComponent::GoToMainMenu() const
{
	dae::SceneManager::GetInstance().SetActiveScene("Menu");
}
