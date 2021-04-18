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
	std::vector<GameObject*> toDelete{};
	for(auto object : m_Objects)
	{
		object->Update();
		if (object->ShouldBeDestroyed())
			toDelete.push_back(object);
	}

	if(toDelete.size() > 0)
	{
		m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(), [&toDelete](GameObject* pObj)
			{
				return std::find(toDelete.begin(), toDelete.end(), pObj) != toDelete.end();
			}), m_Objects.end());

		for (auto object : toDelete)
		{
			m_RenderComponents.erase(std::remove_if(m_RenderComponents.begin(), m_RenderComponents.end(), [&object](RenderComponent* pComp)
				{
					return pComp->GetOwner() == object;
				}));
			delete object;
		}
	}
}

void Scene::Render() const
{
	for (const auto renderComponent : m_RenderComponents)
	{
		renderComponent->Render();
	}
}

