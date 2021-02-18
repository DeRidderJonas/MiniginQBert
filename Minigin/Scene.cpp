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

void Scene::Add(GameObject* object)
{
	m_Objects.push_back(object);
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
	for (const auto object : m_Objects)
	{
		auto renderComponent = object->GetComponentOfType<RenderComponent>();
		if (renderComponent == nullptr) continue;

		renderComponent->Render();
	}
}

