#include "MiniginPCH.h"
#include "GameModeMenuComponent.h"

#include "imgui.h"

dae::GameModeMenuComponent::GameModeMenuComponent(GameObject* pOwner)
	: RenderComponent(pOwner)
{
}

void dae::GameModeMenuComponent::Update()
{
}

void dae::GameModeMenuComponent::Render() const
{
	ImGui::Begin("Modes");
	ImGui::Button("Single Player");
	ImGui::Button("Co-op");
	ImGui::Button("Versus");
	ImGui::End();
}
