#include "MiniginPCH.h"
#include "GameObject.h"
#include "TransformComponent.h"

void dae::GameObject::AddComponent(Component* pComponent)
{
	m_Components.push_back(pComponent);
}

void dae::GameObject::Destroy()
{
	m_ToBeDestroyed = true;
}

bool dae::GameObject::ShouldBeDestroyed() const
{
	return m_ToBeDestroyed;
}

void dae::GameObject::SetScene(Scene* pScene)
{
	m_pScene = pScene;
}

dae::Scene* dae::GameObject::GetScene() const
{
	return m_pScene;
}

dae::GameObject::GameObject()
	: m_ToBeDestroyed(false)
	, m_pScene(nullptr)
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

void dae::GameObject::Initialize()
{
	for(Component* pComponent : m_Components)
	{
		pComponent->Initialize();
	}
}

void dae::GameObject::Update()
{
	for (Component* pComponent : m_Components)
	{
		pComponent->Update();
	}
}
