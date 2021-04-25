#include "MiniginPCH.h"
#include "TransformComponent.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

void dae::TransformComponent::Initialize()
{
}

void dae::TransformComponent::SetPosition(const glm::vec3& pos)
{
	m_Position.x = pos.x;
	m_Position.y = pos.y;
	m_Position.z = pos.z;
}

void dae::TransformComponent::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void dae::TransformComponent::SetPosition(float x, float y)
{
	SetPosition(x, y, 0.f);
}
