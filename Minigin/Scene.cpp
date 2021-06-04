#include "MiniginPCH.h"
#include "Scene.h"

#include <algorithm>

#include "GameContext.h"
#include "GameObject.h"
#include "RenderComponent.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name), m_pGameContext(nullptr) {}

Scene::~Scene()
{
	delete m_pGameContext;
	
	DestroyAll();
}

void Scene::Add(GameObject* pObject)
{
	m_ObjectsToAdd.push_back(pObject);
}

void Scene::SortRenderComponents()
{
	std::sort(m_RenderComponents.begin(), m_RenderComponents.end(), [](RenderComponent* pLeft, RenderComponent* pRight)
		{
			return pLeft->GetRenderPriority() < pRight->GetRenderPriority();
		});
}

void Scene::Update()
{
	if(m_ObjectsToAdd.size() > 0)
	{
		for(auto pGo : m_ObjectsToAdd)
		{
			pGo->SetScene(this);
			pGo->Initialize();

			auto renderComponents = pGo->GetAllComponentsOfType<RenderComponent>();
			for (RenderComponent* pRenderComponent : renderComponents)
			{
				m_RenderComponents.push_back(pRenderComponent);
			}
		}
		m_Objects.insert(m_Objects.end(), m_ObjectsToAdd.begin(), m_ObjectsToAdd.end());
		m_ObjectsToAdd.clear();
		SortRenderComponents();
	}
	
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
			if (m_pGameContext) m_pGameContext->OnRemoveGameObject(object);
			delete object;
		}
	}

	if (m_pGameContext) m_pGameContext->Update();
}

void Scene::Render() const
{
	for (const auto renderComponent : m_RenderComponents)
	{
		renderComponent->Render();
	}
}

void Scene::DestroyAll()
{
	for (auto object : m_Objects)
	{
		delete object;
	}
	m_Objects.clear();

	for (auto object : m_ObjectsToAdd)
	{
		delete object;
	}
	m_ObjectsToAdd.clear();

	m_RenderComponents.clear();
}

const std::string& Scene::GetName() const
{
	return m_Name;
}

void Scene::SetGameContext(GameContext* pGameContext, bool deletePrevious)
{
	if (deletePrevious && m_pGameContext) 
		delete m_pGameContext;
	
	m_pGameContext = pGameContext;
}

GameContext* Scene::GetGameContext() const
{
	return m_pGameContext;
}

