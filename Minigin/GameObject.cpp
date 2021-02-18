#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"

void dae::GameObject::AddComponent(Component* pComponent)
{
	m_Components.push_back(pComponent);
}

dae::GameObject::GameObject()
{
	m_Components.push_back(new TransformComponent(this));
}

dae::GameObject::~GameObject()
{
	for (Component* pComponent : m_Components)
	{
		delete pComponent;
	}
}

void dae::GameObject::Update()
{
	for (Component* pComponent : m_Components)
	{
		pComponent->Update();
	}
}
