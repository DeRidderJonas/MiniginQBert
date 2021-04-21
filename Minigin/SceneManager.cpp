#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}

	//TODO: once GO can be deleted, remove/erase them here
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}

dae::Scene& dae::SceneManager::GetScene(const std::string& name)
{
	auto foundIt = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&name](std::shared_ptr<Scene>& pScene) {return pScene->GetName() == name; });
	if (foundIt != m_Scenes.end()) 
		return **foundIt;

	throw "No scene with name " + name;
}
