#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

void dae::GameObject::AddComponent(const std::shared_ptr<Component>& pComponent)
{
	m_Components.push_back(pComponent);
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(){}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

const dae::Transform& dae::GameObject::GetTransform() const
{
	return m_Transform;
}
