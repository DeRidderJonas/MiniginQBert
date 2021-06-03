#include "MiniginPCH.h"
#include "SceneManager.h"

#include <cassert>
#include "Scene.h"

void dae::SceneManager::Update()
{
	if (m_Scenes.empty())
		return;
	
	m_ActiveScene->Update();
}

void dae::SceneManager::Render()
{
	if (m_Scenes.empty())
		return;
	
	m_ActiveScene->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);

	if (m_Scenes.size() == 1)
		m_ActiveScene = m_Scenes[0];
	
	return *scene;
}

dae::Scene& dae::SceneManager::GetScene(const std::string& name)
{
	auto foundIt = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&name](std::shared_ptr<Scene>& pScene) {return pScene->GetName() == name; });
	if (foundIt != m_Scenes.end()) 
		return **foundIt;

	std::string msg{ "No scene with name " + name };
	assert(!msg.c_str());
	return *m_Scenes[0];
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	auto foundIt = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&name](std::shared_ptr<Scene>& pScene) {return pScene->GetName() == name; });
	if (foundIt != m_Scenes.end())
		m_ActiveScene = *foundIt;
}
