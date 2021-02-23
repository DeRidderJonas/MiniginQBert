#include "MiniginPCH.h"
#include "GameModeMenuComponent.h"

#include "imgui.h"

QBert::GameModeMenuComponent::GameModeMenuComponent(dae::GameObject* pOwner)
	: RenderComponent(pOwner)
{
}

void QBert::GameModeMenuComponent::Update()
{
}

void QBert::GameModeMenuComponent::Render() const
{
	ImGui::Begin("Modes");
	ImGui::Button("Single Player");
	ImGui::Button("Co-op");
	ImGui::Button("Versus");
	ImGui::End();
}
