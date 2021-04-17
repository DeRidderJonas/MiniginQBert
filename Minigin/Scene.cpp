#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "RenderComponent.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene()
{
	for (auto object : m_Objects)
	{
		delete object;
	}
}

void Scene::Add(GameObject* pObject)
{
	m_Objects.push_back(pObject);

	auto renderComponents = pObject->GetAllComponentsOfType<RenderComponent>();
	for (RenderComponent* pRenderComponent : renderComponents)
	{
		m_RenderComponents.push_back(pRenderComponent);
	}
}

void Scene::Add(RenderComponent* pRenderComponent)
{
	m_RenderComponents.push_back(pRenderComponent);
}

void Scene::Update()
{
	for(auto object : m_Objects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto renderComponent : m_RenderComponents)
	{
		renderComponent->Render();
	}
}

